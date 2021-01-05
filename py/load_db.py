from base64 import b64encode
import json
from lxml import html
import requests
import urllib.request as ur



SHORT_TEST_COUNTRIES = [
    'Russia',
]

BIG_TEST_COUNTRIES = [
    'Belgium'
]

FULL_COUNTRIES = [
    'Belgium',
    'Austria',
    'Poland',
    'Russia',
    'USSR',
    'St.-Lucia',
    'Union-Island',
    'St.-Vincent-And-The-Grenadines',
    'St.-Vincent',
    'Grenadines-Of-St.-Vincent',
    'Bequia',
    'Cuba',
    'Tuvalu',
    'Vaitupu',
    'Niutao',
    'Funafuti',
    'Nanumea',
    'Nui',
    'Nukufetau',
]

COUNTRIES = FULL_COUNTRIES

CATEGORIES = [
    None,
    'Railway+parcelpost+stamps',
    'Parcel+post+stamps',
]

URL = 'https://www.stampworld.com'
LINK = URL + '/ru/stamps/COUNTRY/?view=wanted&user=383347'
MAIN = '&cid=164812'
# wanted view=wanted&user=383347&cid=164812
# main view=mycatalogue&cid=164812&user=383347

SERIES_PATH = '/html/body/div[1]/div/div[2]/div/div[1]/div/div[4]/div'
PRICE_PATH = 'div[3]/table/tbody/tr/td[2]/text()'
SERIES_TOP = 'div[1]/div/a/text()'
IMAGES_PATH = 'div[2]/div/span/img/@src'

#


TOP = './/*[@id="topBlueBarGroupsCatalogLabel"]/text()'


def get_country(pre_link, name):
    print(name, ': ')
    link = pre_link.replace('COUNTRY', name)
    country = {}
    for category in CATEGORIES:
        country[category if category is not None else 'default'] = get_category(link, category)
    return country


def get_category(pre_link, name):
    suffix = ''
    if name is not None:
        suffix = '&category_name=' + name
    print('>>\t', (name if name else 'default').replace('+', ' '), ' >> ')
    link = pre_link + suffix
    return request_and_parse(link)


def clrstr(string):
    return string.replace('\\n', '').replace('\t', '').replace(' ', '').replace('\n', '')


ENCODING = 'utf-8'


def parse_response(tree):
    full_prices = []
    series = tree.xpath(SERIES_PATH)
    series_map = []
    for part in series:
        top = part.xpath(SERIES_TOP)
        tops = str(top).replace(' ', '').split('\\n')
        if len(tops) < 4:
            continue

        seria = {}
        seria['year'] = tops[2]
        seria['capture'] = tops[3]

        prices = part.xpath(PRICE_PATH)
        str_prices = [clrstr(str(price)) for price in prices]
        if len(str_prices) > 0:
            full_prices.append(str_prices)
            seria['stamps'] = str_prices

        images = part.xpath(IMAGES_PATH)
        seria['images'] = []
        for image_url in images:
            try:
                raw_data = ur.urlopen(image_url).read()
                b64_bytes = b64encode(raw_data)
                string_data = b64_bytes.decode(ENCODING)
                seria['images'].append(string_data)
            except Exception as err:
                print('load {} failed: {}', image_url, err)


        series_map.append(seria)

    # print(series_map)
    return series_map


def request_and_parse(link):
    # print('\t\t\t\t\t\t', link)
    resp = requests.get(link)
    print(resp.status_code)
    if resp.status_code != 200:
        return []

    list = []

    tree = html.fromstring(resp.content)
    list += parse_response(tree)

    top = str(tree.xpath(TOP)).split('/')
    limit = 0
    if len(top) > 1:
        _limit = top[1][0]
        limit = _limit

    for page in range(2, int(limit) + 1):
        page_link = link + '&page={}'.format(page)
        print('>>\t\tpage {}'.format(page))
        resp = requests.get(page_link)
        if resp.status_code != 200:
            break
        list += parse_response(html.fromstring(resp.content))
    return list


def load():
    data = {}
    for country in COUNTRIES:
        data[country] = get_country(LINK, country)
    return data


data = load()
fout = open('stamp_base.json', 'wt')
data_json = json.dumps(data)
print(data_json)
print(data_json, file=fout)
fout.close()