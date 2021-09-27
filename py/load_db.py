from base64 import b64encode
import json
from lxml import html
import pprint
import requests
import urllib.request as ur


from country_list import *


URL = 'https://www.stampworld.com'
LINK = URL + '/ru/stamps/COUNTRY/?view=wanted&user=383347'
MAIN = '&cid=164812'
# div[2]/div[1]/div/a
SERIES_PATH = '/html/body/div[1]/div/div[2]/div/div[1]/div/div[4]/div'
NUMBER_PATH = 'div[3]/table/tbody/tr/th/a/text()'
TYPE_PATH = 'div[3]/table/tbody/tr/td[1]/a'
PRICE_PATH = 'div[3]/table/tbody/tr/td[2]/text()'
COLOR_PATH = 'div[3]/table/tbody/tr/td[4]/text()'
CAPTURE_PATH = 'div[3]/table/tbody/tr/td[6]/text()'
GET_IMAGE = 'img/@src'
MINT_NEVER_HINGED_PATH = 'div[3]/table/tbody/tr/td[9]'
MINT_PATH = 'div[3]/table/tbody/tr/td[10]'
USED_PATH = 'div[3]/table/tbody/tr/td[11]'
FDC_PATH = 'div[3]/table/tbody/tr/td[12]'
SERIES_TOP = 'div[1]/div/a/text()'
LINK_PATH = 'div[1]/div/a/@href'

IMAGES_PATH = 'div[2]/div/span/' + GET_IMAGE
IMAGES_LINKS = 'div[2]/div/span'
TYPE_TAG = 'data-stamp-type'

TOP = './/*[@id="topBlueBarGroupsCatalogLabel"]/text()'


def get_country(pre_link, name):
    print('\n', name, ': ')
    link = pre_link.replace('COUNTRY', name)
    country = {
        'default': get_category(link, None),
    }
    if name in CATEGORIES_SETTINGS:
        for category in CATEGORIES_SETTINGS[name]:
            country[category] = get_category(link, category)
    return country


def get_category(pre_link, name):
    suffix = ''
    if name is not None:
        suffix = '&category_name=' + name
    print('>> ', (name if name else 'default').replace('+', ' '))
    link = pre_link + suffix
    print('>>    wishlist')
    res = request_and_parse(link)
    print('>>    ownlist')
    res += request_and_parse(link + MAIN, my=True)
    return res



def clrstr(string):
    return string.replace('\\n', '').replace('\t', '').replace('  ', '').replace('\n', '')


ENCODING = 'utf-8'


def parse_response(tree, my):
    series = tree.xpath(SERIES_PATH)
    series_map = []
    for i, part in enumerate(series):
        print('\rLoad series {} of {}              '.format(i+1, len(series)), end="\r")
        top = part.xpath(SERIES_TOP)
        tops = str(top).replace('  ', '').split('\\n')
        if len(tops) < 4:
            continue

        link = part.xpath(LINK_PATH)

        seria = {'year': tops[2], 'capture': tops[3], 'sw_link': link[0]}

        prices = part.xpath(PRICE_PATH)
        str_prices = [clrstr(str(price)) for price in prices]

        if len(str_prices) == 0:
            continue

        numbers = part.xpath(NUMBER_PATH)
        str_numbers = [clrstr(str(num)) for num in numbers]

        captures = part.xpath(CAPTURE_PATH)
        str_caps = [clrstr(str(cap)) for cap in captures]

        colors = part.xpath(COLOR_PATH)
        str_colors = [clrstr(str(col)) for col in colors]

        types = part.xpath(TYPE_PATH)
        str_types = [
            clrstr(str(typ.xpath('text()')).replace('[', '').replace(']', '')
                   .replace("'", ''))
            for typ in types
        ]

        mnhs = part.xpath(MINT_NEVER_HINGED_PATH)
        mints = part.xpath(MINT_PATH)
        useds = part.xpath(USED_PATH)
        fdcs = part.xpath(FDC_PATH)

        images = part.xpath(IMAGES_PATH)
        images_map = {}
        for image in images:
            code = image.split('/')[-1].split('-')[0]
            if code[0] == '0':
                code = code.replace('0', '')
            images_map[code] = image

        if TEST:
            print('pre_images: ', images)
            print('images: ', [k for k in images_map.keys()])
            print('nums: ', str_numbers)

        assert len(str_numbers) == len(str_prices)
        assert len(str_numbers) == len(str_caps)
        assert len(str_numbers) == len(str_types)
        assert len(str_numbers) == len(str_colors)

        assert len(str_numbers) == len(mnhs)
        assert len(str_numbers) == len(mints)
        assert len(str_numbers) == len(useds)
        assert len(str_numbers) == len(fdcs)

        unique_nums = set()
        num_range = None
        for num in str_numbers:
            if '‑' not in num:
                unique_nums.add(num)
            else:
                num_range = num
        has_list = len(unique_nums) != len(str_numbers)
        full_size = len(unique_nums)
        if has_list:
            seria['has_list'] = True
        indexes = list(range(len(str_numbers)))
        if num_range:
            boarders = num.split('‑')
            low = int(boarders[0].replace('A', '').replace('B', ''))
            up = int(boarders[1].replace('A', '').replace('B', ''))
            full_size = up - low + 1
            for ind in range(len(indexes)):
                if '‑' not in str_numbers[ind]:
                    try:
                        indexes[ind] = int(str_numbers[ind].replace('A', '').replace('B', '')) - low
                    except:
                        pass
                    finally:
                        pass

        stamps = {}
        for num, typ, price, cap, color, mnh, mint, used, fdc, index in zip(
                str_numbers, str_types, str_prices, str_caps, str_colors,
                mnhs, mints, useds, fdcs, indexes
        ):
            if TEST:
                print('# {}, type {}, price {}'.format(num, typ, price))
            if '‑' not in num and num not in stamps:
                stamps[num] = {}
            if typ != '':
                stamp = {
                    'type': typ,
                    'price': price,
                    'capture': cap,
                    'color': color,
                    'owned': my,
                }
                if has_list:
                    stamp['list_note'] = '{} of {}'. format(index+1, full_size)
                if my:
                    stamp['cond'] = []
                    if len(str(mnh.xpath(GET_IMAGE))) > 2:
                        # print(len(str(mnh.xpath(GET_IMAGE))))
                        stamp['cond'].append('mnh')
                    if len(str(mint.xpath(GET_IMAGE))) > 2:
                        stamp['cond'].append('mint')
                    if len(str(used.xpath(GET_IMAGE))) > 2:
                        stamp['cond'].append('used')
                    if len(str(fdc.xpath(GET_IMAGE))) > 2:
                        stamp['cond'].append('fdc')
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
                if my:
                    seria['list_cond'] = []
                    if len(str(mnh.xpath(GET_IMAGE))) > 2:
                        # print(len(str(mnh.xpath(GET_IMAGE))))
                        seria['list_cond'].append('mnh')
                    if len(str(mint.xpath(GET_IMAGE))) > 2:
                        seria['list_cond'].append('mint')
                    if len(str(used.xpath(GET_IMAGE))) > 2:
                        seria['list_cond'].append('used')
                    if len(str(fdc.xpath(GET_IMAGE))) > 2:
                        seria['list_cond'].append('fdc')
                    seria['list_owned'] = len(seria['list_cond']) > 0

                if '‑' not in num:
                    if num in images_map:
                        stamps[num]['image'] = images_map[num]
                else:
                    boarders = num.split('‑')
                    if TEST:
                        print('filling images for ', boarders)
                    low = int(boarders[0].replace('A', '').replace('B', ''))
                    up = int(boarders[1].replace('B', '').replace('A', ''))
                    if (up - low) > 30:
                        continue
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
        series_map.append(seria)

    # print(series_map)
    print()
    return series_map


def request_and_parse(link, my=False):
    print('>>      request ', link, end='\t\t')

    resp = requests.get(link)
    print(resp.status_code)
    assert (resp.status_code == 200, 'REQUEST ERROR! {}'.format(resp.status_code))

    _list = []

    tree = html.fromstring(resp.content)
    # tout = open('resp.html', 'wt')
    # print(resp.content, file=tout)
    # tout.close()

    _list += parse_response(tree, my)

    top = str(tree.xpath(TOP)).split('/')
    limit = 0
    if len(top) > 1:
        _limit = top[1][0]
        limit = _limit

    for page in range(2, int(limit) + 1):
        page_link = link + '&page={}'.format(page)
        print('>>      page {} of {}'.format(page, int(limit)), end='\t\t')
        print(page_link, end='\t\t')
        resp = requests.get(page_link)
        print(resp.status_code)
        assert (resp.status_code == 200, 'REQUEST ERROR! {}'.format(resp.status_code))
        _list += parse_response(html.fromstring(resp.content), my)
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
