#########################################################################
# File Name: tag.sh
# Author: Juven
# mail: yeashenlee@163.com
# Created Time: 2015年09月22日 星期二 13时53分19秒
#########################################################################
#!/bin/bash

# generate tag file for lookupfile plugin
echo -e "!_TAG_FILE_SORTED\t2\t/2=foldcase/" > filenametags
find . -not -regex '.*\.\(png\|gif\)' -type f -printf "%f\t%p\t1\n" | \
	    sort -f >> filenametags 
