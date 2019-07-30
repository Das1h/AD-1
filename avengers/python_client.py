# -*- coding:utf-8 -*-
import socket
import DBHandler

dbc = DBHandler.DBHandler(host='127.0.0.1', port=12345)

if dbc.connect():
    print("Connected")
    print(dbc.request('get_all value 0'))
    print(dbc.request('find key 1'))
    print(dbc.request('find value 2'))
    print(dbc.request('find time_stamp 201907262359'))
    print(dbc.request('insert 999 unko'))
    print(dbc.request('delete key 999'))
else:
    print("Disconnected")
