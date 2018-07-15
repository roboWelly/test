#!/bin/bash

git init

echo "Enter project name: "
read projName

git add .
git pull "https://github.com/roboWelly/$projName"

git remote rm origin
git remote add origin "https://github.com/roboWelly/$projName"

echo "Enter commit number: "
read commitNum

git commit -m "Commit $commitNum"
git push origin master
