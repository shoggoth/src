#!/usr/bin/env sh
git status && git branch &&\
read -p "Commit description: " description &&\
git add . && git add -u && git commit -m "$description" &&\
read -p "Push to:  " push_location &&\
git push $push_location
