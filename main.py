import json
import http

from lxml import html
import requests

import urllib.request as ur

COUNTRIES = [
    'Austria',
    'Poland',
    'Russia',
    'USSR',
]

LINK = 'https://www.stampworld.com/ru/stamps/COUNTRY/?view=wanted&user=383347&cid=164812'
#class="table table-striped table-sm table-hover data_table"
#
PRICE_PATH = '/html/body/div[1]/div/div[2]/div/div[1]/div/div[4]/div/div[3]/table/tbody/tr/td[2]/text()'


def get_country(name):
    print(name, ': ')
    link = LINK.replace('COUNTRY', name)
    print(link)
    resp = requests.get(link)
    print(resp.status_code)
    tree = html.fromstring(resp.content)
    print(tree)
    print(tree.xpath(PRICE_PATH))

def main():
    for country in COUNTRIES:
        get_country(country)




if __name__ == '__main__':
    main()


