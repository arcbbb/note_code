#!/bin/bash

git filter-branch --commit-filter \
	    'if [ "$GIT_AUTHOR_NAME" = "arcbbb" ]; then \
	    export GIT_AUTHOR_NAME="arcbbb";\
	    export GIT_AUTHOR_EMAIL=arcbbb@gmail.com;\
	    export GIT_COMMITTER_NAME="arcbbb";\
	    export GIT_COMMITTER_EMAIL=arcbbb@gmail.com;\
	    fi;\
	    git commit-tree "$@"'
