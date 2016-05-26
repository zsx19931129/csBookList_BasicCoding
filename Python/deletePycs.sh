#! /bin/bash

find . -name "*.pyc" | xargs -I {} echo \"{}\" | xargs rm -rf