#!/bin/sh

case "$#" in
    [2])
        ;;
    *)
        echo "Usage: ${0##*/} ClassName symbolfile"
        exit 1
        ;;
esac

CLASSNAME=$1
SYMBOLTABLE=$2
rm ./"$CLASSNAME".h

echo "#ifndef __"$CLASSNAME"_H__" | cat >> ./"$CLASSNAME".h
echo "#define __"$CLASSNAME"_H__" | cat >> ./"$CLASSNAME".h
echo "" | cat >> ./"$CLASSNAME".h

echo "class $CLASSNAME" | cat >> ./"$CLASSNAME".h
echo "{" | cat >> ./"$CLASSNAME".h
echo "public:" | cat >> ./"$CLASSNAME".h
echo "" | cat >> ./"$CLASSNAME".h

cat $SYMBOLTABLE | egrep $CLASSNAME"::" | sed "s/"$CLASSNAME":://" | cat >> ./"$CLASSNAME".h
echo "};" | cat >> ./"$CLASSNAME".h
echo "" | cat >> ./"$CLASSNAME".h
echo "#endif //__"$CLASSNAME"_H__" | cat >> ./"$CLASSNAME".h

