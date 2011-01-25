#!/bin/sh

qemuopts="-hda obj/kern/kernel.img"
. ./grade-functions.sh


$make

check () {
	pts=20
	echo_n "Page directory: "
	if grep "check_boot_pgdir() succeeded!" jos.out >/dev/null
	then
		pass
	else
		fail
	fi

	pts=30
	echo_n "Page management: "
	if grep "page_check() succeeded!" jos.out >/dev/null
	then
		pass
	else
		fail
	fi
}

run
check

showfinal
