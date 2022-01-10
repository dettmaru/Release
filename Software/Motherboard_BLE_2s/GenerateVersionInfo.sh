#!/bin/sh
FILE="Src/Utilities/VersionInfo.cpp"

gitHash=$(git rev-parse HEAD)
gitBranch=$(git branch | grep \* | cut -d ' ' -f2-)
date=$(date)

cat <<EOM >$FILE
/*
 * VersionInfo.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Daniel Lohmann
 */
#include "VersionInfo.h"

char VersionInfo[] = "Git Hash: ${gitHash}\nGit Branch: ${gitBranch}";
char CompileDate[] = "Compile date: ${date}";

#ifdef DEBUG
char BuildConfiguration[] = "Build configuration: DEBUG";
#else
char BuildConfiguration[] = "Build configuration: RELEASE";
#endif
EOM
