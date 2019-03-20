#! /bin/sh
#
# multiCursor.sh
# Copyright (C) 2019 g <g@ABCL>
#
# Distributed under terms of the MIT license.
#


xinput create-master second
# Get ID of touchpad
touchID=$( (
	xinput list 2>/dev/null
	true
) | grep Touchpad | cut -f 2 | cut -d'=' -f 2)
# Get ID of master

masterID=$( (
	xinput list 2>/dev/null
	true
) | grep second | grep master | grep pointer | cut -f 2 | cut -d'=' -f 2)

xinput reattach $touchID $masterID
