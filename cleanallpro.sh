#在windows使用此脚本需要装windows Git
#!/bin/sh
#用于删除qt项目的用户配置文件*.user*
for file in `find -name "*.user*"`
do
 rm $file 
done
#用于删除qt项目的build-*文件夹
for file in `find -name "build-*"`
do
 rm -rf $file 
done