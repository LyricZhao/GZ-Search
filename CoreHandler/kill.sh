#!/usr/bin/env bash
ps | grep CoreHandler | grep -v grep | cut -c 1-5 | xargs kill -9
