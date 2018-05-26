from django.http import HttpResponse
from django.shortcuts import render
from django.shortcuts import render_to_response
import os
import ctypes
import hashlib
from ctypes import *

AllNewsResult = []
AllAnswerResult = []
so = ctypes.cdll.LoadLibrary
lib = so("/Users/stdafx/Desktop/Assignment/GZ Searcher/SearchSite/Glue/CXXHandler.so")

def init():
    # print("init")
    lib.init()

def searchStart(request):
    return render_to_response("StartPage.html")

def receiveInput(request):
    AllNewsResult.clear()
    AllAnswerResult.clear()
    textDict = {}
    textDict['input'] = request.GET.get('searchText', False)
    # print(textDict['input'])
    input_p = c_wchar_p(textDict['input'])
    so = ctypes.cdll.LoadLibrary
    # lib = so("C:\Guyuxian\Discrete Mathematics(2)\GZSearcher\CXXHandler\CXXHandler.so")
    # ctypes.create_unicode_buffer
    lib.CXXSearch(input_p, len(textDict['input']))
    n = lib.ResultSize()
    textDict['cxxinput'] = input_p.value
    textDict['n'] = n

    for i in range(0, n):
        c_i = c_int(i)

        #compute title
        pyGetTitle = lib.GetTitle
        pyGetTitle.argtypes = [c_int]
        pyGetTitle.restype = c_char_p
        btitle = pyGetTitle(c_i)
        title = btitle.decode()

        #compute url
        pyGetUrl = lib.GetUrl
        pyGetUrl.argtypes = [c_int]
        pyGetUrl.restype = c_char_p
        burl = pyGetUrl(c_i)
        url = burl.decode()

        #compute context
        pyGetContext = lib.GetContext
        pyGetContext.argtypes = [c_int]
        pyGetContext.restype = c_char_p
        bcontext = pyGetContext(c_i)
        context = bcontext.decode()

        #compute type
        type = lib.GetType(c_i)
        result = {'title': title, 'url': url, 'context': context}
        if type == 0:
            AllNewsResult.append(result)
        else:
            AllAnswerResult.append(result)

    return render(request, 'SeriousAnswer.html', {'answers': AllAnswerResult, 'number': len(AllAnswerResult)})

def showSeriousAnswer(request):
    return render(request, 'SeriousAnswer.html', {'answers': AllAnswerResult, 'number': len(AllAnswerResult)})

def showNews(request):
    return render(request, 'News.html', {'answers':AllNewsResult, 'number': len(AllNewsResult)});
