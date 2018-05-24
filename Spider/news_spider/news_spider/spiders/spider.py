# -*- coding: UTF-8 -*-

'''
GZ Searcher
News Spider Core By Lyric
'''

import re
import zmq
import scrapy

from unicodedata import numeric
from scrapy.spiders import Spider

def is_number(ch):
    try:
        numeric(ch)
        return True
    except (TypeError, ValueError):
        pass
    return False

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

class newsSpider(Spider):

    send_counter = 0
    name = "news"
    allowed_domains = ['chinanews.com',]
    headers = { 'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36', }

    # index_dic = {} # scrapy has made this function defaultly enabled

    # @GZ_TYPE@...@GZ_TITLE@...@GZ_URL@...@GZ_CONTEXT@...@GZ_RELATED@...@GZ_END@

    def decodeSender(self, type, title, url, context, related):
        if len(title) < 1 or len(url) < 1:
            return
        print "sending data ..."
        sendContext = "@GZ_TYPE@" + str(type) + "@GZ_TITLE@" + title + "@GZ_URL@" + url + "@GZ_CONTEXT@" + context + "@GZ_RELATED@" + related + "@GZ_END@"
        sendContext = re.sub(u'\s', '', sendContext)
        # print sendContext
        ZMQ_Swaper.sendData(sendContext)
        print "send ok !"
        # debug_loop()


    def plog(self, log_str):
        self.logger.info(log_str)
        print log_str

    def start_requests(self):
        '''
        # DEBUG:
        debug_url = 'http://www.chinanews.com/shipin/2014/04-18/report430.shtml'
        yield scrapy.Request(url = debug_url, callback = self.parse_news, headers = self.headers)
        '''
        start_url = 'http://www.chinanews.com/'

        yield scrapy.Request(url = start_url, callback = self.parse_index, headers = self.headers)


    def is_article(self, url):
        number_counter = 0
        for i in range(0, len(url)):
            number_counter += is_number(url[i])
        return (number_counter >= 5)

    def links_process(self, all_links, curr = ""):
        news = []
        indexs = []
        for url in all_links:
            if 'javascript:ongetkey' in url:
                pattern = re.findall(u'\d', url)
                if '?pager=' in curr:
                    curr = curr[ : curr.find('?pager=')]
                url = curr + "?pager=" + pattern[0]
                indexs.append(url)
                continue
            if not (('.html' in url) or ('.shtml' in url)):
                continue
            if url.startswith('//'):
                url = "http:" + url
            elif url.startswith('/'):
                url = "http://www.chinanews.com" + url
            if self.is_article(url):
                news.append(url)
            else:
                indexs.append(url)
        return news, indexs

    def judge_link_density(self, response):
        links_size = len(response.xpath('//@href').extract())
        words = response.xpath('//p/text()').extract()
        words_len = 0
        for word in words:
            words_len += len(word)
        return words_len > links_size * 2

    def contextExtractor(self, response):
        divs = response.xpath('//div')
        context = []
        for div in divs:
            subdiv = div.xpath('.//div').extract()
            if len(subdiv):
                continue
            if self.judge_link_density(div): # Judge OK
                context.extend(div.xpath('.//p/text()').extract())
        for words in context:
            print words
        return context

    def getContext(self, response):
        # news
        news_title = response.xpath('//div[@class="con_left"]//h1/text()').extract()
        if len(news_title):
            context_extract = response.xpath('//div[@class="left_zw"]/p/text()').extract()
            related = response.xpath('//ul[@class="padding-left20"]//a/@href').extract()
            return 0, news_title[0], context_extract, related

        # videos
        video_title = response.xpath('//div[@class="video_con"]/h1/text()').extract()
        if len(video_title):
            return 1, video_title[0], [], []

        # picture
        pic_title = response.xpath('//div[@id="_playpic"]/i[@class="title"]/text()').extract()
        if len(pic_title):
            context_extract = response.xpath('//div[@id="cont_show"]//div[@class="t3"]/text()').extract()
            return 2, pic_title[0], context_extract, []

        # live
        live_title = response.xpath('//div[@class="titleWrap"]/h1/text()').extract()
        if len(live_title):
            context_extract = response.xpath('//div[@class="description"]/p/text()').extract()
            related = response.xpath('//ul[@class="newslist"]//a/@href').extract()
            return 3, live_title[0], context_extract, related

        # Unknown
        title = response.xpath('/html/head/title/text()').extract()[0]
        all_following_links = response.xpath('//@href').extract()
        news, indexs = self.links_process(all_following_links)
        context_extract = self.contextExtractor(response);
        return -1, title, context_extract, news

    def parse_index(self, response):
        self.plog('Requesting Index Page: %s' % response.url)
        '''
        if response.url in self.index_dic:
            yield None
        self.index_dic[response.url] = 1
        '''
        all_following_links = response.xpath('//@href').extract()
        news, indexs = self.links_process(all_following_links, response.url)

        # index page forward
        for index in indexs:
            yield scrapy.Request(url = index, callback = self.parse_index, headers = self.headers)

        # context page
        for news_page in news:
            yield scrapy.Request(url = news_page, callback = self.parse_news, headers = self.headers)

    def parse_news(self, response):
        self.plog('Requesting Article Page: %s' % response.url)
        contextType, title, context, related = self.getContext(response)
        # Unknown
        if contextType == -1:
            self.plog(u'Analyzing unknown page OK, Title: %s' % title)

        # News
        elif contextType == 0:
            self.plog(u'Analyzing news page OK, Title: %s' % title)

        # Videos
        elif contextType == 1:
            self.plog(u'Analyzing video page OK, Title: %s' % title)

        # Picture
        elif contextType == 2:
            self.plog(u'Analyzing picture page OK, Title: %s' % title)

        # Live
        elif contextType == 2:
            self.plog(u'Analyzing live page OK, Title: %s' % title)

        # in some cases the url format is not good
        for i in range(0, len(related)):
            if related[i].startswith("//"):
                related[i] = "http:" + related[i]
            elif related[i].startswith("www"):
                related[i] = "http://" + related[i]

        # Send to CoreHandler
        self.send_counter += 1
        print 'Trying to send the %d msg.' % self.send_counter
        self.decodeSender(0, title, response.url, "".join(context), ",".join(related))