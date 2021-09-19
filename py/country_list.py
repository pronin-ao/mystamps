TEST = False

EUROPA = [
    'Austria',  # !
    'Belarus',  # !
    'Belgium',
    'Bulgaria',
    'Great-Britain',
    'Hungary',
    'DDR',
    'Germany',
    'German-Empire',
    'Bohemia-and-Moravia',
    'Greece',
    'Denmark',
    'Ireland',
    'Spain',
    'Italy',
    'San-Marino',
    'Latvia',  # !
    'Lithuania',  # !
    'Luxembourg',
    'Malta',
    'Moldova',
    'Monaco',
    'Netherlands',
    'Norway',
    'Isle-of-Man',
    'Poland',
    'Portugal',
    'Russia',  # !
    'Romania',
    'Serbia',
    'Serbian-Rep.-B-and-H',
    'Slovakia',
    'USSR',
    'Turkey',
    'Ukraine',  # !
    'Finland',  # !
    'France',
    'Croatia',
    'Czechoslovakia',
    'Czech-Republic',
    'Switzerland',
    'Sweden',
    'Estonia',  # !
    'Yugoslavia',
    'Albania',  # !
    'Kosovo',   # !
    'Saar',
    'Saargebiet',  # !
]

AFRICA = [
    'UAR',
    'Belgian-Congo',  # !
    'Benin',
    'French-West-Africa',
    'Ivory-Coast',
    'Burkina-Faso',
    'Upper-Volta',
    'Burundi',
    'Gabon',
    'Ghana',
    'Guinea',
    'Guinea-Bissau',
    'Djibouti',
    'French-Somali-Coast',
    'Zambia',
    'Northern-Rhodesia',
    'Zimbabwe',
    'Southern-Rhodesia',
    'Cameroon',
    'Comoro-Islands',
    'Congo,-Dr',
    'Congo,-Rep.',
    'Lesotho',
    'Liberia',
    'Mauritius',
    'Madagascar',
    'Malawi',
    'Mali',
    'Mozambique',
    'Mozambique-Company',
    'Namibia',
    'South-West-Africa',
    'Niger',
    'Nigeria',
    'Sao-Tome-And-Principe',
    'Sierra-Leone',
    'Tanzania',
    'Kenya,-Uganda-and-Tanganyika',
    'Togo',
    'Uganda',
    'Chad',
    'French-Equatorial-Africa',
    'Middle-Congo',
    'Equatorial-Guinea',
    'South-Africa',
    'South-African-Republic',
    'Belgian-Occ.-of-German-East-Africa',  # !
    'Seychelles',
    'Tunisia',
]

NORTH_AMERICA = [
    'Antigua',
    'Antigua-And-Barbuda',
    'Barbuda',
    'New-Brunswick',
    'Newfoundland',
    'Guatemala',
    'Honduras',
    'Grenada',
    'Grenada-Grenadines',
    'Dominica',
    'Dominican-Republic',
    'Canada',
    'Cuba',
    'Mexico',
    'Montserrat',
    'Nevis',
    'Nicaragua',
    'Panama',
    'Canal-Zone',
    'El-Salvador',
    'St.-Vincent-And-The-Grenadines',
    'St.-Vincent',
    'Grenadines-Of-St.-Vincent',
    'Bequia',
    'Union-Island',
    'St.-Kitts',
    'St.-Lucia',
    'United-States',  # !
    'Jamaica',
    'Trinidad-And-Tobago',
]

SOUTH_AMERICA = [
    'Argentina',
    'Bolivia',
    'Brazil',
    'Venezuela',
    'Guyana',
    'Colombia',
    'Paraguay',
    'Peru',
    'Uruguay',
    'Chile',
    'Ecuador',
    'Falkland-Islands',
]

ASIA = [
    'Azerbaijan',
    'Afghanistan',
    'Bangladesh',
    'Bhutan',
    'Vietnam',
    'North-Vietnam',
    'South-Vietnam',
    'Israel',
    'India',
    'Indonesia',
    'Iran',
    'Kazakhstan',
    'Cambodia',
    'China,-Peoples-Rep.',
    'Taiwan',
    'East-China',
    'Northeast-China',
    'China,-Empire',
    'Kyrgyzstan',
    'Laos',
    'Malaysia',
    'North-Borneo',
    'Maldives',
    'Mongolia',
    'United-Arab-Emirates',
    'Ajman',
    'Umm-al-Qiwain',
    'Sharjah',
    'Pakistan',
    'North-Korea',
    'Thailand',
    'Philippines',
    'Sri-Lanka',
    'Ceylon',
    'Japan',  # ?
    'Manama',
    'Ras-al-Khaima',
    'Umm-al-Qiwain',
    'Fujeira',
    'Armenia',
    'Bahrain',
    'Pahang',
    'Uzbekistan',
    'Jordan',
    'Indochina',
    'Hong-Kong'
]

AUSTRALIA = [
    'Australia',
    'Christmas-Island',
    'Micronesia',
    'Niue',
    'New-Zealand',
    'Pitcairn-Islands',
    'Tonga',
    'Tuvalu',
    'Vaitupu',
    'Nanumaga',
    'Niutao',
    'Funafuti',
    'Nanumea',
    'Nui',
    'Nukufetau',
    'Nukulaelae',
]

FULL_COUNTRIES = []

FULL_COUNTRIES += EUROPA
FULL_COUNTRIES += AFRICA
FULL_COUNTRIES += NORTH_AMERICA
FULL_COUNTRIES += SOUTH_AMERICA
FULL_COUNTRIES += ASIA
FULL_COUNTRIES += AUSTRALIA

if TEST:
    COUNTRIES = ['Fujeira', 'Russia']
else:
    COUNTRIES = FULL_COUNTRIES

CATEGORIES_SETTINGS = {
    'Belgium': ['Railway+parcelpost+stamps', 'Parcel+post+stamps'],
    'United-States': ['Parcel+post+stamps'],
    'France': ['Parcel+post+stamps'],
    'Monaco': ['Postage-due+stamp'],
    'Bolivia': ['Postal+tax+stamps'],
    'North-Vietnam': ['Military+stamps']
}
