#!/bin/bash
echo $(amixer get Speaker |grep -E "Left:" |awk '{print $4}')
