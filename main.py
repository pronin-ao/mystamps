'''from lxml import html
import requests



COUNTRIES = [
    'Belgium',
    'Austria',
    'Poland',
    'Russia',
    'USSR',
]
CATEGORIES = [
    None,
    'Railway+parcelpost+stamps'
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
        seria['images'] = images

        series_map.append(seria)

    # print(series_map)
    return series_map


def request_and_parse(link):
    # print('\t\t\t\t\t\t', link)
    resp = requests.get(link)
    print(resp.status_code)
    if resp.status_code != 200:
        return

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

'''

# import test_data as tdata
import sys
import urllib.request as ur
from PyQt5.QtWidgets import QApplication, QWidget, QLabel
from PyQt5.QtGui import QPixmap

VER = '3'

LINK = 'http://swmedia-4cd6.kxcdn.com/media/catalogue/USSR/Postage-stamps/ZW-s.jpg'

'''
from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlApplicationEngine
def window():
    # app = QApplication(sys.argv)

    app = QApplication(sys.argv)

    # Create a QML engine.
    engine = QQmlApplicationEngine('main.qml')
    print('0')
    # engine.load()
    print('1')

    # Create a component factory and load the QML script.
    # component = QQmlComponent(engine)
    # print('2')
    # component.loadUrl(QUrl('main.qml'))
    # print('3')

    # Create an instance of the component.
    # qml = component.create()
    print('4')

    if engine is not None:
        sys.exit(app.exec_())
        '''

def test_window():
    app = QApplication(sys.argv)
    widget = QWidget(windowTitle="Stamps((")
    widget.setStyleSheet('background-color: blue;')
    text_label = QLabel('TEXT ' + VER, widget)
    link = LINK
    print('!!! 1')
    try:
        # data = ur.urlopen(link).read()
        image = QPixmap(link)
        # image.loadFromData(data)
        text_label.setPixmap(image)
    except Exception as err:
        text_label.setText('FAILED: ', err)

    text_label.move(0, 0)
    text_label.resize(300, 80)
    text_label.setStyleSheet('background-color: black; color: cyan;')
    widget.show()
    ret = app.exec_()
    sys.exit(ret)

test_window()


# data = load()

# data = tdata.const_data()
# print(data)
# gui.test_window(data)

# gui.window(data)
