#!/bin/sh
echo $(amixer get Headphone |grep -E "Left:" |awk '{print $4}')
