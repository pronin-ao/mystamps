from base64 import b64encode
import json
from lxml import html
import pprint
import requests
import urllib.request as ur

TEST = False

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
    'Nevis',
]

if TEST:
    COUNTRIES = ['Belgium']
else:
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
NUMBER_PATH = 'div[3]/table/tbody/tr/th/a/text()'
TYPE_PATH = 'div[3]/table/tbody/tr/td[1]/a'
CAPTURE_PATH = 'div[3]/table/tbody/tr/td[6]/text()'
SERIES_TOP = 'div[1]/div/a/text()'

IMAGES_PATH = 'div[2]/div/span/img/@src'
IMAGES_LINKS = 'div[2]/div/span'
TYPE_TAG = 'data-stamp-type'

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
    return string.replace('\\n', '').replace('\t', '').replace('  ', '').replace('\n', '')


ENCODING = 'utf-8'


def parse_response(tree):
    series = tree.xpath(SERIES_PATH)
    series_map = []
    for part in series:
        top = part.xpath(SERIES_TOP)
        tops = str(top).replace('  ', '').split('\\n')
        if len(tops) < 4:
            continue

        seria = {'year': tops[2], 'capture': tops[3]}

        prices = part.xpath(PRICE_PATH)
        str_prices = [clrstr(str(price)) for price in prices]

        if len(str_prices) == 0:
            continue

        numbers = part.xpath(NUMBER_PATH)
        str_numbers = [clrstr(str(num)) for num in numbers]

        captures = part.xpath(CAPTURE_PATH)
        str_caps = [clrstr(str(cap)) for cap in captures]

        types = part.xpath(TYPE_PATH)
        str_types = [
            clrstr(str(typ.xpath('text()')).replace('[', '').replace(']', '')
                   .replace("'", ''))
            for typ in types
        ]

        images = part.xpath(IMAGES_PATH)
        images_map = {}
        for image in images:
            code = image.split('/')[-1].split('-')[0]
            if code[0] == '0':
                code = code.replace('0', '')
            images_map[code] = image

        if TEST:
            print('images: ', [k for k in images_map.keys()])

        assert len(str_numbers) == len(str_prices)
        assert len(str_numbers) == len(str_caps)
        assert len(str_numbers) == len(str_types)

        stamps = {}
        for num, typ, price, cap in zip(str_numbers, str_types, str_prices, str_caps):
            if TEST:
                print('# {}, type {}, price {}'.format(num, typ, price))
            if '‑' not in num and num not in stamps:
                stamps[num] = {}
            if typ != '':
                stamp = {
                    'type': typ,
                    'price': price,
                    'capture': cap,
                }
                if typ in images_map:
                    stamp['image'] = images_map[typ]
                elif len(images_map) == 1:
                    if TEST:
                        print('WARN: setting only image for {}'.format(num))
                    stamp['image'] = list(images_map.values())[0]
                elif TEST:
                    print('WARN: ', typ, num, ' not in map')
                stamps[num] = stamp
            else:
                if '‑' not in num:
                    if num in images_map:
                        stamps[num]['image'] = images_map[num]
                else:
                    boarders = num.split('‑')
                    if TEST:
                        print('filling images for ', boarders)
                    low = int(boarders[0])
                    up = int(boarders[1])
                    for i in range(low, up+1):
                        num = str(i)
                        if num not in stamps:
                            print('skipping absent ', num)
                            continue
                        if boarders[0] in images_map:
                            stamps[num]['image'] = images_map[boarders[0]]
                        elif TEST:
                            print('{} expected to be in images for {} but not'.format(low, num))

        seria['stamps'] = stamps

        for key, stamp in seria['stamps'].items():
            if 'image' not in stamp:
                print('!!!ERROR stamp with no image: ', key, stamp)
                continue
            image_url = stamp['image']
            try:
                if not TEST:
                    raw_data = ur.urlopen(image_url).read()
                    b64_bytes = b64encode(raw_data)
                    string_data = b64_bytes.decode(ENCODING)
                    stamp['image'] = string_data
            except Exception as err:
                print('load {} failed: {}'.format(image_url, err))

        series_map.append(seria)

    # print(series_map)
    return series_map


def request_and_parse(link):
    print('\t\t\t\t\t\t', link)

    resp = requests.get(link)
    print(resp.status_code)
    if resp.status_code != 200:
        return []

    _list = []

    tree = html.fromstring(resp.content)
    # tout = open('resp.html', 'wt')
    # print(resp.content, file=tout)
    # tout.close()

    _list += parse_response(tree)

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
        _list += parse_response(html.fromstring(resp.content))
    return _list


def load():
    _data = {}
    for country in COUNTRIES:
        _data[country] = get_country(LINK, country)
    return _data


data = load()
data_json = json.dumps(data)
if not TEST:
    fout = open('stamp_base.json', 'wt')
    print(data_json, file=fout)
    fout.close()
else:
    # pp = pprint.PrettyPrinter(indent=4)
    # pp.pprint(data_json)
    print(data_json)
