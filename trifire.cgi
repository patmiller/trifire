#!/usr/bin/env python3
# -*- coding: UTF-8 -*-# enable debugging
# -*-python-*-

import ast
import os
import sys
import json
import MySQLdb
import trifire

from cgi_util import get_json_post

# This turns on a Python trace capture (ignoring DeprecationWarning)
import warnings
with warnings.catch_warnings():
    warnings.filterwarnings("ignore", category=DeprecationWarning)
    import cgi
    import cgitb
    cgitb.enable()


try:
    data = get_json_post()
    if not data:
        print(os.environ)
    command = data.get('command','x')
except Exception as e:
    print('Content-Type: text/html;charset=utf-8')
    print()
    print(str(e))
    sys.exit(0)

# The requests purports to have access to this database
USER = os.getenv('USER','www-data')

try:
    db = MySQLdb.connect(host='localhost',user=USER,db='trifire')
except:
    print('Content-Type: text/html;charset=utf-8')
    print()
    print('db?')
    sys.exit(0)

with db.cursor() as cur:
    sql = 'SELECT turn,state FROM state ORDER BY turn DESC LIMIT 1'
    try:
        cur.execute(sql)
        turn,state = cur.fetchone()
    except:
        print('Content-Type: text/html;charset=utf-8')
        print()
        print('db fetch?')
        sys.exit(0)

    state = ast.literal_eval(state)

    import trifire
    try:
        new_state = trifire.play(state,command[0])
    except RuntimeError as e:
        print('Content-Type: text/html;charset=utf-8')
        print()
        print(state)
        print(e)
        sys.exit(0)

    sql = 'INSERT INTO state VALUES (%s,%s)'
    try:
        cur.execute(sql,(new_state['turn'],new_state,))
    except:
        raise
        print('Content-Type: text/html;charset=utf-8')
        print()
        print('db update?')
        sys.exit(0)
    db.commit()


print("Status: 302 Found")
print("Location: https://luxcedia.icu/trifire.html")
print()
