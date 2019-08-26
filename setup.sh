QuickPath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo '#ifndef QuickPath' > $QuickPath/inc/currentPath.hh
echo '#define QuickPath "'${QuickPath}'"' >> $QuickPath/inc/currentPath.hh
echo "#endif" >> $QuickPath/inc/currentPath.hh
