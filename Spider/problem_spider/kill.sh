ps | grep /usr/bin/python | grep -v grep | cut -c 1-5 | xargs kill -9
