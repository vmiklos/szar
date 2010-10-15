#!/bin/bash

if [ ! -e omninames-$(hostname).log ]; then
	omniNames -start -logdir .
else
	omniNames -logdir .
fi
