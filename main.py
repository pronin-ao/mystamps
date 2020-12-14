import json
import http

from lxml import html
import requests

import urllib.request as ur

COUNTRIES = [
    'Belgium'#,
    #'Austria',
    #'Poland',
    #'Russia',
    #'USSR',
]
CATEGORIES = [
    None,
    'Railway+parcelpost+stamps'
]

URL = 'https://www.stampworld.com'
LINK = URL+'/ru/stamps/COUNTRY/?view=wanted&user=383347'
MAIN = '&cid=164812'
#class="table table-striped table-sm table-hover data_table"
# wanted view=wanted&user=383347&cid=164812
# main view=mycatalogue&cid=164812&user=383347
PRICE_PATH = '/html/body/div[1]/div/div[2]/div/div[1]/div/div[4]/div/div[3]/table/tbody/tr/td[2]/text()'

TOP = './/*[@id="topBlueBarGroupsCatalogLabel"]/text()'


def get_country(pre_link, name):
    print(name, ': ')
    link = pre_link.replace('COUNTRY', name)
    for category in CATEGORIES:
        get_category(link, category)


def get_category(pre_link, name):
    suffix = ''
    if name is not None:
        suffix = '&category_name='+name
    print('>>\t', (name if name else 'default').replace('+', ' '), ' >> ')
    link = pre_link + suffix
    request_and_print(link)


def print_response(tree):
    prices = tree.xpath(PRICE_PATH)
    str_prices = [(str(price)).replace('\n','').replace('\t','').replace(' ','') for price in prices]
    print(str_prices)
    print()


def request_and_print(link):
    print('\t\t\t\t\t\t', link)
    resp = requests.get(link)
    print(resp.status_code)
    if resp.status_code != 200:
        return

    tree = html.fromstring(resp.content)
    print_response(tree)


    top = str(tree.xpath(TOP)).split('/')
    limit = 0
    if len(top) > 1:
        _limit = top[1][0]
        print('LIMIT = ', _limit)
        limit = _limit

    for page in range(2, int(limit) + 1):
        page_link = link + '&page={}'.format(page)
        print('>>\t\tpage {}'.format(page))
        resp = requests.get(page_link)
        if resp.status_code != 200:
            break
        print_response(html.fromstring(resp.content))



def main():
    for country in COUNTRIES:
        get_country(LINK, country)


if __name__ == '__main__':
    main()


