git filter-branch -env-filter '
OLD_EMAIL="<xiali123.ch@gmail.com>"
CORRECT_EMAIL="xiali123.ch@gmail.com"

if [ "$GIT_COMMITTER_EMAIL" = "$OLD_EMAIL" ]
then
    export GIT_COMMITTER_EMAIL="$CORRECT_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$OLD_EMAIL" ]
then
    export GIT_AUTHOR_EMAIL="$CORRECT_EMAIL"
fi
' -tag-name-filter cat — -branches -tags