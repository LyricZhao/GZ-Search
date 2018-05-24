# -*- coding: UTF-8 -*-

'''
GZ Searcher
Problem Spider Core By Lyric
'''

import re
import zmq
import time
import scrapy
import datetime

from unicodedata import numeric
from scrapy.spiders import Spider

def debug_loop():
    while(True):
        pass
    return

class MsgSwap:
    def __init__(self):
        self.port = "4723"
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://localhost:%s" % self.port)

    def sendData(self, strData):
        print "Sending:"
        print strData
        self.socket.send_string(strData)
        feedbackMSG = self.socket.recv()

ZMQ_Swaper = MsgSwap()

class problemSpider(Spider):

    name = "problem"

    allowed_domains = ['zikao365.com',
                       '9120.cn',
                       'examw.com']

    # zikao365.com 9120.cn examw.com

    headers = { 'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36', }
    crawlList = {
        'ZIKAO365' : True,
        '9120': True,
        'EXAMW': True,
    }

    def decodeSender(self, type, title, url, context, related):
        sendContext = "@GZ_TYPE@" + str(type) + "@GZ_TITLE@" + title + "@GZ_URL@" + url + "@GZ_CONTEXT@" + context + "@GZ_RELATED@" + related + "@GZ_END@"
        ZMQ_Swaper.sendData(sendContext)

    def plog(self, log_str):
        self.logger.info(log_str)
        print log_str

    def start_requests(self):
        # zikao365.com
        if self.crawlList['ZIKAO365']:
            ltime = time.localtime()
            date = "%04d-%02d-%02d" % (ltime.tm_year, ltime.tm_mon, ltime.tm_mday)
            # SX
            start_url0_pre = "http://member.zikao365.com/zikao-qz/create/daytest/getQues4NoCache.shtm?Forum_id=1454&method=1&d="
            url = start_url0_pre + date
            yield scrapy.Request(url = url, callback = self.parse_365, headers = self.headers)
            # LG
            start_url0_pre = "http://member.zikao365.com/zikao-qz/create/daytest/getQues4NoCache.shtm?Forum_id=1451&method=1&d="
            url = start_url0_pre + date
            # yield scrapy.Request(url = url, callback = self.parse_365, headers = self.headers)
            # MY
            start_url0_pre = "http://member.zikao365.com/zikao-qz/create/daytest/getQues4NoCache.shtm?Forum_id=1452&method=1&d="
            url = start_url0_pre + date
            # yield scrapy.Request(url = url, callback = self.parse_365, headers = self.headers)

        # 9120.cn
        if self.crawlList['9120']:
            start_urls_pre = 'http://www.9120.cn/m/list.php?tid=48&TotalResult=5469&PageNo='
            for i in range(1, 548):
                page_url = start_urls_pre + str(i)
                yield scrapy.Request(url = page_url, callback = self.parse_9120_page, headers = self.headers)

        # examw.com
        if self.crawlList['EXAMW']:
            startURL = 'http://www.examw.com/zikao/note/Gonggong/'
            yield scrapy.Request(url = startURL, callback = self.parse_examw_index, headers = self.headers)
        # end

    def getTimeofURL(self, url):
        # example: 2018-03-31
        ulen = len(url)
        year = int(url[ulen - 10 : ulen - 6])
        mon  = int(url[ulen - 5 : ulen - 3])
        day  = int(url[ulen - 2 : ulen])
        return year, mon, day

    def mergeStr(self, strs):
        allText = ""
        for text in strs:
            allText += text + '\n'
        return allText

    def isLineEmpty(self, singleLine):
        for c in singleLine:
            if not (c == ' ' or c == '\t' or c == '\n' or c == '\r' or c == u'\xa0'):
                return False
        return True

    def problemSplit(self, mainContext):
        lines = mainContext.splitlines()
        emptyCounter = 0
        currentProblem = ""
        problems = []
        for line in lines:
            if self.isLineEmpty(line):
                emptyCounter += 1
            else:
                if emptyCounter >= 3:
                    if len(currentProblem) > 0:
                        problems.append(currentProblem)
                    currentProblem = ""
                    emptyCounter = 0
                currentProblem += line + '\n'
        problems.append(currentProblem)
        return problems

    def problemAnalyzer(self, mainContext):
        problems = self.problemSplit(mainContext)
        '''
        print "LEN = " + str(len(problems))
        for problem in problems:
            print '#### Begin ####'
            print problem
            print '#### End ####'
        debug_loop()
        '''
        return problems

    def parse_365(self, response):
        url = response.url
        extractContent = response.xpath('//div[@class="dt_wt"]/div/text()').extract()
        if len(extractContent) < 2:
            yield None
        else:
            self.plog(u'Analyzing problem from zikao365.com, URL: %s' % url)
            question = extractContent[1]
            # print question
            selection = response.xpath('//div[@class="saveOneQuestion dt_xxbg"]/p/label/text()').extract()
            # for sel in selection:
            #    print sel
            answer = response.xpath('//div[@class="rA"]/text()').extract()[1]
            # print answer
            analyse = response.xpath('//div[@class="ana"]/text()').extract()[1]
            # print analyse
            # type[01], title, url, context, related
            allContext = question
            for sel in selection:
                allContext += sel
            allContext += answer
            self.decodeSender(1, question, url, allContext, "")

            year, mon, day = self.getTimeofURL(response.url)
            curDate = datetime.datetime(year, mon, day)
            preDate = curDate - datetime.timedelta(days = 1)
            strDate = preDate.strftime("%Y-%m-%d")
            nextURL = url[0 : len(url) - 10] + strDate
            # print nextURL
            yield scrapy.Request(url = nextURL, callback = self.parse_365, headers = self.headers)

    def parse_9120_page(self, response):
        url = response.url
        self.plog(u'Analyzing Index page 9120.cn, URL: %s' % url)

        pages = response.xpath('//h3[@class="am-list-item-hd"]/a/@href').extract()
        pages_title = response.xpath('//h3[@class="am-list-item-hd"]/a//text()').extract()
        if len(pages) != len(pages_title):
            # Error
            debug_loop()

        for i in range(0, len(pages)):
            page, title = pages[i], pages_title[i]
            if not u'考试真题及答案解析' in title:
                continue
            page_index = "http://www.9120.cn/m/" + page + "&pageno=1"
            yield scrapy.Request(url = page_index, callback = self.parse_9120_problem, headers = self.headers)

    def parse_9120_problem(self, response):
        url = response.url
        self.plog(u'Analyzing Problem page 9120.cn, URL: %s' % url)

        # Handling problem
        mainContextArr = response.xpath('//div[@class="am-g blog-content"]//td/p/text()').extract()
        mainContext = self.mergeStr(mainContextArr)
        problems = self.problemAnalyzer(mainContext)
        for problem in problems:
           self.decodeSender(1,  problem, url, problem, "")

        # Jump to next page
        curPage = int(url[url.find('&pageno=') + 8 : len(url)])
        pages_tot_str = response.xpath('//div[@class="dede_pages"]//ul[@data-am-widget="pagination"]/li/a/text()').extract_first()
        pages_tot_str = pages_tot_str[1 : len(pages_tot_str) - 3]
        pages_tot = int(pages_tot_str)
        if curPage != pages_tot:
            curPage += 1
            nextURL = url[0 : url.find('&pageno=') + 8] + str(curPage)
            yield scrapy.Request(url = nextURL, callback = self.parse_9120_problem, headers = self.headers)

    def parse_examw_index(self, response):
        self.plog(u'Analyzing index page www.examw.com')

        pages = response.xpath('//div[@class="box21 brRight"]/ul/li/a/@href').extract()
        for page in pages:
            nextURL = "http://www.examw.com" + page
            yield scrapy.Request(url = nextURL, callback = self.parse_examw_cate, headers = self.headers)

    def examw_getIndex(self, url):
        indexPos = url.find('index-')
        if indexPos == -1:
            return 1
        else:
            numberStr = url[indexPos + 6 : url.find('.html')]
            indexNo = int(numberStr)
            return indexNo

    def examw_nextPage(self, url):
        indexPos = url.find('index-')
        if indexPos != -1:
            numberStr = url[indexPos + 6 : url.find('.html')]
            indexNo = int(numberStr)
            indexNo += 1
            return url[0 : indexPos] + 'index-' + str(indexNo) + '.html'
        else:
            return url + 'index-2.html'

    def parse_examw_cate(self, response):
        url = response.url
        self.plog(u'Analyzing category page: %s' % url)

        pages = response.xpath('//div[@class="List_Main"]/div/li/a/@href').extract()
        for page in pages:
            nextURL = "http://www.examw.com" + page
            yield scrapy.Request(url = nextURL, callback = self.parse_examw_note, headers = self.headers)

        rollPage = response.xpath('//div[@id="fy"]/a/@href').extract()
        if len(rollPage) != 0:
            lastPage = "http://www.examw.com" + rollPage[len(rollPage) - 1]
            if self.examw_getIndex(url) < self.examw_getIndex(lastPage):
                nextURL = self.examw_nextPage(url)
                # print "next_page: " + next_page
                yield scrapy.Request(url = nextURL, callback = self.parse_examw_cate, headers = self.headers)

    def parse_examw_note(self, response):
        url = response.url
        self.plog(u'Analyzing note page: %s' % url)

        contextTitle = response.xpath('//div[@id="News"]/div[@class="title"]/h3/text()').extract_first()
        contextMain = response.xpath('//div[@id="NewsBox"]/p/text()').extract()

        self.decodeSender(1,  contextTitle, url, ''.join(contextMain), "")

        rollPage = response.xpath('//div[@class="page"]/a/@hreft').extract()
        if len(rollPage) != 0:
            lastPage = "http://www.examw.com" + rollPage[len(rollPage) - 1]
            if self.examw_getIndex(url) < self.examw_getIndex(lastPage):
                nextURL = self.examw_nextPage(url)
                # print "next_page: " + next_page
                yield scrapy.Request(url = nextURL, callback = self.parse_examw_note, headers = self.headers)
