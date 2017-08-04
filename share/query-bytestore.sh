#!/bin/tcsh -ef
# author : sjn and apr
# date : Aug. 2017

set usage_text = 'Usage:\
\
    query-bytestore.sh [ --mutual-regions ]\
                       [ --xfac2015 | --uniprobe | --taipale | --jaspar | --hg38-dnaseI | --mm10-dnaseI ]\
                         --within-range A:B | --outside-range A:B\
                         <bed-file>\
\
    For score range thresholding, -1 <= A <= 1 and -1 <= B <= 1.\
\
    The optional --mutual-regions argument restricts output to interval pairs\
    that mutually overlap where they overlap <bed-file>.\
\
    At least and at most one database option should be selected (TRANSFAC 2015,\
    UniProbe, Taipale, etc.).\
\
    The <bed-file> argument should be a regular file, containing three-column\
    BED interval data: chromosome, start, and stop positions.\
'

set temp=(`getopt -s tcsh -o hmxujtdMw:o: --long help,mutual-regions,xfac2015,uniprobe,jaspar,taipale,hg38-dnaseI,mm10-dnaseI,within-range:,outside-range: -- $argv:q`)
if ($? != 0) then
    echo "Terminating..." >/dev/stderr
    exit 1
endif

eval set argv=\($temp:q\)

@ mutual = 0
@ db_cnt = 0
@ filter_cnt = 0

set db=""
set filter=""
set filter_range=""

while (1)
    switch($1:q)
    case -h:
    case --help:
        echo $usage_text:q;
        exit 0;
    case -m:
    case --mutual-regions:
        @ mutual = 1;
        shift;
        breaksw;
    case -x:
    case --xfac2015:
        set db = "xfac2015";
        @ db_cnt += 1
        shift;
        breaksw;
    case -u:
    case --uniprobe:
        set db = "uniprobe";
        @ db_cnt += 1
        shift;
        breaksw;
    case -j:
    case --jaspar:
        set db = "jaspar";
        @ db_cnt += 1
        shift;
        breaksw;
    case -t:
    case --taipale:
        set db = "taipale";
        @ db_cnt += 1
        shift;
        breaksw;
    case -d:
    case --hg38-dnaseI:
        set db = "hg38-dnaseI";
        @ db_cnt += 1
        shift;
        breaksw;
    case -M:
    case --mm10-dnaseI:
        set db = "mm10-dnaseI";
        @ db_cnt += 1
        shift;
        breaksw;
    case -w:
    case --within-range:
        if ($2:q == "") then
            echo "Error: Please specify A:B range";
            echo $usage_text:q;
            exit 1;
        endif
        set filter = "--score-filter-ranged-within-inclusive";
        set filter_range = $2:q
        @ filter_cnt += 1
        shift;
        shift;
        breaksw;
    case -o:
    case --outside-range:
        if ($2:q == "") then
            echo "Error: Please specify A:B range";
            echo $usage_text:q;
            exit 1;
        endif
        set filter = "--score-filter-ranged-outside-exclusive";
        set filter_range = $2:q
        @ filter_cnt += 1
        shift;
        shift;
        breaksw;
    case --:
        shift;
        break;
    default:
        echo "Internal error!";
        exit 1;
    endsw
end

# if more or less than one filter type is specified, print usage and quit
if ($filter_cnt != 1) then
    echo "Error: Please specify at least and no more than one filter type"
    echo $usage_text:q
    exit 1
endif

# if more or less than one database is specified, print usage statement and quit early
if ($db_cnt != 1) then
    echo "Error: Please specify at least and no more than one database"
    echo $usage_text:q
    exit 1
endif

# if bed file is unspecified, print usage statement and quit early
if ($#argv != 1) then
    echo "Error: Please specify <bed-file>"
    echo $usage_text:q
    exit 1
endif
set query_file = $1:q
shift

# validate filter range
set r1 = `echo "$filter_range" | cut -f1 -d':'`
set r2 = `echo "$filter_range" | cut -f2 -d':'`
@ ok1 = `echo "$r1" | awk '(-1 <= $1 && $1 <= 1) { print NR }'`
@ ok2 = `echo "$r2" | awk '(-1 <= $1 && $1 <= 1) { print NR }'`
if ($ok1 == 0 || $ok2 == 0 || $r1 == $r2) then
    echo "Error: Bad range A:B given with filter"
    echo $usage_text:q
    exit 1
endif

set tmpdir = /tmp/`whoami`/bquery/$$

rm -rf $tmpdir
mkdir -p $tmpdir

set query_file_master_coords = $tmpdir/.mz

set base_dir = "/net/seq/data/projects/bytestore"
set bs_dir = "$base_dir/github/byte-store"
set bs_binary = "$bs_dir/byte-store"
set bs_binary_httpd_lib_path = "$bs_dir/third-party/libmicrohttpd-0.9.53/lib"

@ llp = 1
foreach ld_lib_path ( $LD_LIBRARY_PATH:as/:/ / )
  if ( -f "$ld_lib_path/libmicrohttpd.so" ) then
    @ llp = 0
  endif
end
if ( $llp == 1 ) then
  setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH\:"$bs_binary_httpd_lib_path"
endif

if ( ! -s $query_file ) then
  echo "Error: Cannot find your <bed-file>"
  echo $usage_text:q
  exit 1
else if ( ! -s $bs_binary ) then
  echo "Error: Cannot find the byte-store query binary"
  echo $usage_text:q
  exit 1
endif

set master = ""
set store = ""
set db_type = ""

if ($db == "xfac2015") then
    set master = "/net/seq/data/projects/bytestore/827_master_list_v061417a_cross_3143_motifs_xfac2015/jaccard/prerequisites/results/master_with_row_indices.bed"
    set store = "/net/seq/data/projects/bytestore/827_master_list_v061417a_cross_3143_motifs_xfac2015/jaccard/production/results/827_master_list_v061417a_cross_3143_motifs_xfac2015.50000r.bs"
    set db_type = "jaccard-index-sqr-split"
else if ($db == "uniprobe") then
    set master = "/net/seq/data/projects/bytestore/827_master_list_v061417a_cross_408_motifs_uniprobe/jaccard/prerequisites/results/master_with_row_indices.bed"
    set store = "/net/seq/data/projects/bytestore/827_master_list_v061417a_cross_408_motifs_uniprobe/jaccard/production/results/827_master_list_v061417a_cross_408_motifs_uniprobe.25000r.bs"
    set db_type = "jaccard-index-sqr-split"    
else if ($db == "jaspar") then
    set master = "/net/seq/data/projects/bytestore/827_master_list_v061417a_cross_107_motifs_jaspar/jaccard/prerequisites/results/master_with_row_indices.bed"
    set store = "/net/seq/data/projects/bytestore/827_master_list_v061417a_cross_107_motifs_jaspar/jaccard/production/results/827_master_list_v061417a_cross_107_motifs_jaspar.25000r.bs"
    set db_type = "jaccard-index-sqr-split"
else if ($db == "taipale") then
    set master = "/net/seq/data/projects/bytestore/827_master_list_v061417a_cross_755_motifs_taipale/jaccard/prerequisites/results/master_with_row_indices.bed"
    set store = "/net/seq/data/projects/bytestore/827_master_list_v061417a_cross_755_motifs_taipale/jaccard/production/results/827_master_list_v061417a_cross_755_motifs_taipale.25000r.bs"
    set db_type = "jaccard-index-sqr-split"    
else if ($db == "hg38-dnaseI") then
    set master = "/net/seq/data/projects/bytestore/827_master_list_v061417a/pearson/prerequisites/results/master_with_row_indices.bed"
    set store = "/net/seq/data/projects/bytestore/827_master_list_v061417a/pearson/production/results/827_master_list_v061417a.100000r.bs"
    set db_type = "pearson-r-sqr-split"    
else if ($db == "mm10-dnaseI") then
    set master = "/net/seq/data/projects/bytestore/224_mouse_master_list_v070817a/pearson/prerequisites/results/master_with_row_indices.bed"
    set store = "/net/seq/data/projects/bytestore/224_mouse_master_list_v070817a/pearson/production/results/224_mouse_master_list_v070817a.50000r.bs"
    set db_type = "pearson-r-sqr-split"
endif

bedextract $master $query_file \
  | awk 'BEGIN {fst=-99; lst=-99} ; { \
          if ( int($4) != lst+1 ) { \
            if ( lst >= 0 ) { \
              print fst"-"lst; \
            } \
            fst = int($4); \
            lst = int($4); \
          } else { \
            lst = int($4); \
          } \
        } END { \
          if ( lst >= 0 ) { \
            print fst"-"lst; \
          } \
        }' \
 >! $query_file_master_coords

set mutual_query = ""

if ($mutual == 1) then
    set mutual_query = "--mutual-query"
endif

if ( -s $query_file_master_coords ) then
    $bs_binary -t $db_type -q -l $master -s $store -z $query_file_master_coords $filter $filter_range $mutual_query | sort-bed --max-mem 2G -
endif
  
rm -rf $tmpdir

exit 0