#!/usr/bin/env python

import sys
import subprocess
import os
import datetime
import errno
import shutil

this_task = "bytestore"
parent_project_dir = "/net/seq/data/projects/bytestore/786_master_list_v011117a/test"

input_bed = "master_10k.bed"
input_rows = 10000
input_bed_path = os.path.join(parent_project_dir, input_bed)

bs_binary = "byte-store"
bs_binary_path = os.path.join(parent_project_dir, "bin", bs_binary)

slurm_create_blocks_wrapper = "parallel_store_creation_blocks_via_slurm_submit.sh"
slurm_create_blocks_wrapper_path = os.path.join(parent_project_dir, slurm_create_blocks_wrapper)

slurm_create_metadata_wrapper = "parallel_store_creation_metadata_via_slurm_submit.sh"
slurm_create_metadata_wrapper_path = os.path.join(parent_project_dir, slurm_create_metadata_wrapper)

if os.path.isfile(input_bed_path):
    sys.stderr.write("Debug: input_bed_path [%s]\n" % (input_bed_path))
else:
    raise IOError("Error: Could not find input BED file\n")

if os.path.isfile(bs_binary_path):
    sys.stderr.write("Debug: bs_binary_path [%s]\n" % (bs_binary_path))
else:
    raise IOError("Error: Could not find byte-store binary\n")

if os.path.isfile(slurm_create_blocks_wrapper_path):
    sys.stderr.write("Debug: slurm_create_blocks_wrapper_path [%s]\n" % (slurm_create_blocks_wrapper_path))
else:
    raise IOError("Error: Could not find SLURM create-blocks wrapper\n")

if os.path.isfile(slurm_create_metadata_wrapper_path):
    sys.stderr.write("Debug: slurm_create_metadata_wrapper_path [%s]\n" % (slurm_create_metadata_wrapper_path))
else:
    raise IOError("Error: Could not find SLURM create-blocks wrapper\n")

# we encode in 512-row chunks
encoding_row_step = 512
encoding_row_initial_position = 0
encoding_row_position_offsets = list(xrange(encoding_row_initial_position, input_rows, encoding_row_step))
encoding_dest_prefix = "master_10k_slurm.512r.bs"
encoding_dest_prefix_path = os.path.join(parent_project_dir, encoding_dest_prefix)
encoding_dest_real_path = os.path.join(parent_project_dir, ".".join([encoding_dest_prefix, "blocks"]))
try:
    os.makedirs(encoding_dest_real_path)
except OSError as err:
    if err.errno == errno.EEXIST and os.path.isdir(encoding_dest_real_path):
        shutil.rmtree(encoding_dest_real_path)
        os.makedirs(encoding_dest_real_path)
    else:
        raise

#
# SLURM parameters
#

slurm_job_ids = list()
slurm_job_ids.append("afterok")

#
# Blocks
#

slurm_partition = 'queue1'
slurm_mem_per_cpu = 1000
slurm_cpus_per_task = 1
slurm_ntasks = 1
slurm_ntasks_per_node = 1

for encoding_row_offset in encoding_row_position_offsets:
    task_cmd_submitted = datetime.datetime.utcnow().isoformat() + 'Z'
    task_cmd_components = [
        'sbatch',
        '--parsable',
        '--partition=' + slurm_partition,
        '--mem-per-cpu=' + str(slurm_mem_per_cpu),
        '--cpus-per-task=' + str(slurm_cpus_per_task),
        '--ntasks=' + str(slurm_ntasks),
        '--ntasks-per-node=' + str(slurm_ntasks_per_node),
        '--workdir=' + encoding_dest_real_path
    ]
    task_cmd_jobname = "_".join([this_task, input_bed, str(encoding_row_step), str(encoding_row_offset)])
    task_cmd_components.append('--job-name=' + task_cmd_jobname)
    task_cmd_components.append(slurm_create_blocks_wrapper_path)
    task_cmd_components.append(bs_binary_path)
    task_cmd_components.append(input_bed_path)
    task_cmd_components.append(encoding_dest_prefix_path)
    task_cmd_components.append(str(encoding_row_step))
    task_cmd_components.append(str(encoding_row_offset))
    task_cmd = ' '.join(task_cmd_components)
    sys.stderr.write("Debug: Per-block command [%s]\n" % (task_cmd))
    
    try:
        env = os.environ.copy()
        env['PATH'] = '/usr/local/bin:' + env['PATH']
        task_cmd_result = subprocess.check_output(task_cmd, shell=True, env=env)
    except subprocess.CalledProcessError as err:
        task_cmd_result = "Error: Command '{}' returned with error (code {}): {}".format(err.cmd, err.returncode, err.output)
        
    slurm_job_id = task_cmd_result.rstrip()
    slurm_job_ids.append(slurm_job_id)

#
# Metadata
#

slurm_partition = 'queue1'
slurm_mem_per_cpu = 100
slurm_cpus_per_task = 1
slurm_ntasks = 1
slurm_ntasks_per_node = 1

task_cmd_components = [
    'sbatch',
    '--parsable',
    '--partition=' + slurm_partition,
    '--mem-per-cpu=' + str(slurm_mem_per_cpu),
    '--cpus-per-task=' + str(slurm_cpus_per_task),
    '--ntasks=' + str(slurm_ntasks),
    '--ntasks-per-node=' + str(slurm_ntasks_per_node),
    '--workdir=' + encoding_dest_real_path,
    '--dependency=' + ':'.join(slurm_job_ids)
]
task_cmd_jobname = "_".join([this_task, input_bed, "metadata"])
task_cmd_components.append('--job-name=' + task_cmd_jobname)
task_cmd_components.append(slurm_create_metadata_wrapper_path)
task_cmd_components.append(bs_binary_path)
task_cmd_components.append(input_bed_path)
task_cmd_components.append(encoding_dest_prefix_path)
task_cmd_components.append(str(encoding_row_step))
task_cmd = ' '.join(task_cmd_components)
sys.stderr.write("Debug: Metadata command: [%s]\n" % (task_cmd))

try:
    env = os.environ.copy()
    env['PATH'] = '/usr/local/bin:' + env['PATH']
    task_cmd_result = subprocess.check_output(task_cmd, shell=True, env=env)
except subprocess.CalledProcessError as err:
    task_cmd_result = "Error: Command '{}' returned with error (code {}): {}".format(err.cmd, err.returncode, err.output)

slurm_job_id = task_cmd_result.rstrip()
