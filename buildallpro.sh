#此脚本只适用于linux下使用
source /opt/fsl-imx-x11/4.1.15-2.1.0/environment-setup-cortexa7hf-neon-poky-linux-gnueabi
#!/bin/bash
path=$(pwd)
# 1
echo "building QClock...."
cd $path/QClock/QClock
rm QClock
sync
qmake
make -j 16
make clean
rm Makefile
#2
echo "building QCalendar...."
cd $path/QCalendar/QCalendar
rm QCalendar
sync
qmake
make -j 16
make clean
rm Makefile
#3
echo "building QFileView...."
cd $path/QFileView/QFileView
rm QFileView
sync
qmake
make -j 16
make clean
rm Makefile
#4
echo "building QLed...."
cd $path/QLed/QLed
rm QLed
sync
qmake
make -j 16
make clean
rm Makefile
#5
echo "building QMusicPlayer...."
cd $path/QMusicPlayer/QMusicPlayer
rm QMusicPlayer
sync
qmake
make -j 16
make clean
rm Makefile
#6
echo "building QOnlineWeather...."
cd $path/QOnlineWeather/QOnlineWeather
rm QOnlineWeather
sync
qmake
make -j 16
make clean
rm Makefile
7
echo "building QReader...."
cd $path/QReader/QReader
rm QReader
sync
qmake
make -j 16
make clean
rm Makefile
#8
echo "building QCalculator...."
cd $path/QCalculator/QCalculator
rm QCalculator
sync
qmake
make -j 16
make clean
rm Makefile
#9
echo "building QCamera...."
cd $path/QCamera/QCamera
rm QCamera
sync
qmake
make -j 16
make clean
rm Makefile
#10
echo "building QDesktop...."
cd $path/QDesktop/QDesktop
rm QDesktop
sync
qmake
make -j 16
make clean
rm Makefile
#11
echo "building QImageViewer...."
cd $path/QImageViewer/QImageViewer
rm QImageViewer
sync
qmake
make -j 16
make clean
rm Makefile
#12
echo "building QMediaPlayer...."
cd $path/QMediaPlayer/QMediaPlayer
rm QMediaPlayer
sync
qmake
make -j 16
make clean
rm Makefile
#13
echo "building QNotepad...."
cd $path/QNotepad/QNotepad
rm QNotepad
sync
qmake
make -j 16
make clean
rm Makefile
#14
echo "building QPictureViewer...."
cd $path/QPictureViewer/QPictureViewer
rm QPictureViewer
sync
qmake
make -j 16
make clean
rm Makefile
#15
echo "building QRecorder...."
cd $path/QRecorder/QRecorder
rm QRecorder
sync
qmake
make -j 16
make clean
rm Makefile
#16
echo "building QTemperature...."
cd $path/QTemperature/QTemperature
rm QTemperature
sync
qmake
make -j 16
make clean
rm Makefile
#17
echo "building QWebBrowser...."
cd $path/QWebBrowser/QWebBrowser
rm QWebBrowser
sync
qmake
make -j 16
make clean
rm Makefile
#178
echo "building QAutotest...."
cd $path/QAutotest/QAutotest
rm QAutotest
sync
qmake
make -j 16
make clean
rm Makefile


#image update
cp $path/QWebBrowser/QWebBrowser/QWebBrowser 		$path/qt5.5.1/apps/QWebBrowser
cp $path/QTemperature/QTemperature/QTemperature 	$path/qt5.5.1/apps/QTemperature
cp $path/QRecorder/QRecorder/QRecorder 			$path/qt5.5.1/apps/QRecorder
cp $path/QPictureViewer/QPictureViewer/QPictureViewer 	$path/qt5.5.1/apps/QPictureViewer
cp $path/QNotepad/QNotepad/QNotepad 			$path/qt5.5.1/apps/QNotepad
cp $path/QMediaPlayer/QMediaPlayer/QMediaPlayer 	$path/qt5.5.1/apps/QMediaPlayer
cp $path/QImageViewer/QImageViewer/QImageViewer 	$path/qt5.5.1/apps/QImageViewer
cp $path/QDesktop/QDesktop/QDesktop 			$path/qt5.5.1/apps/QDesktop
cp $path/QCamera/QCamera/QCamera 			$path/qt5.5.1/apps/QCamera
cp $path/QCalculator/QCalculator/QCalculator 		$path/qt5.5.1/apps/QCalculator
cp $path/QReader/QReader/QReader 			$path/qt5.5.1/apps/QReader
cp $path/QOnlineWeather/QOnlineWeather/QOnlineWeather 	$path/qt5.5.1/apps/QOnlineWeather
cp $path/QMusicPlayer/QMusicPlayer/QMusicPlayer 	$path/qt5.5.1/apps/QMusicPlayer
cp $path/QLed/QLed/QLed					$path/qt5.5.1/apps/QLed
cp $path/QFileView/QFileView/QFileView 			$path/qt5.5.1/apps/QFileView
cp $path/QClock/QClock/QClock 				$path/qt5.5.1/apps/QClock
cp $path/QCalendar/QCalendar/QCalendar 			$path/qt5.5.1/apps/QCalendar
cp $path/QAutotest/QAutotest/QAutotest                 	$path/qt5.5.1/apps/QAutotest

echo "构建完成，请把qt5.5.1这个文件夹替换到文件系统的/opt目录下"
cd $path