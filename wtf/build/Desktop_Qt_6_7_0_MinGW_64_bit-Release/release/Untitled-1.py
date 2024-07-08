import requests
import re
from prettytable import PrettyTable
from colorama import Fore
import ssl

ssl._create_default_https_context = ssl._create_unverified_context
def getStationName():
    # 爬取12306网站所有车站名称信息
    
    url = 'https://www.12306.cn/index/script/core/common/station_name_v10026.js'  # 车站信息控件
    r = requests.get(url)
    pattern = '([\u4e00-\u9fa5]+)\|([A-Z]+)'  # 正则匹配规则
    result = re.findall(pattern, r.text)
    stationName = dict(result)  # 所有车站信息，转换为字典

    return stationName

def get_price_url(text, date, from_station, to_station):
    # 构建用于查询列车车次信息的url
    # 参数：日期，出发地，到达地
    # key为车站名称， value为车站代号

    date = date
    from_station = text[from_station]
    to_station = text[to_station]    # 新的url
    
    url = ("https://kyfw.12306.cn/otn/leftTicketPrice/query?leftTicketDTO.train_date={}"
           "&leftTicketDTO.from_station={}"
           "&leftTicketDTO.to_station={}"
           ).format(date, from_station, to_station)
    return url

def get_price(date, from_station, to_station):
    text = getStationName()
    url = get_price_url(text, date, from_station, to_station)

    headers = {
        'Cookie': f'_jc_save_toStation={text[from_station]}',
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36 Edg/108.0.1462.76',
    }

    r = requests.get(url=url, headers=headers)

    results = r.json()['data']

    tb = PrettyTable()
    tb.field_names = ['车次',  "起点站", "起点站代号", "终点站", "终点站代号",   "出发站", "出发站代号", "到站", "到站代号",
                      '开始时', '结束时', '持续时间',
                      '商务座',  '一等座', '二等座 ', '高级软卧', '软卧', '硬卧', '软座',   '硬座', '站票']

    acc = []

    for i in results:
        traindata = i["queryLeftNewDTO"]
        
        tb.add_row([ 
            traindata["station_train_code"],
            traindata["start_station_name"],
            traindata["start_station_telecode"],
            traindata["end_station_name"],
            traindata["end_station_telecode"],
            traindata["from_station_name"],
            traindata["from_station_telecode"],
            traindata["to_station_name"],
            traindata["to_station_telecode"],
            traindata["start_time"],
            traindata["arrive_time"],
            traindata["lishi"],
            traindata["swz_price"], # 商务座
            traindata["zy_price"], # 一等座
            traindata["ze_price"], # 二等座
            traindata["gr_price"], # 高级软卧
            traindata["rw_price"], # 软卧
            traindata["yw_price"], # 硬卧
            traindata["rz_price"], # 软座
            traindata["yz_price"], # 硬座
            traindata["wz_price"], # 无座
        ])
        acc.append(
            f"{traindata['from_station_name']} {traindata['to_station_name']} {traindata['station_train_code']} "
            f"{traindata['start_time']} {traindata['arrive_time']} {traindata['lishi']} "
            f"{traindata['swz_price'] or '--'} {traindata['zy_price'] or '--'} {traindata['ze_price'] or '--'} "
            f"{traindata['gr_price'] or '--'} {traindata['rw_price'] or '--'} {traindata['yw_price'] or '--'} "
            f"{traindata['rz_price'] or '--'} {traindata['yz_price'] or '--'} {traindata['wz_price'] or '--'}"
        )


    with open('train_schedule.txt', 'w', encoding='utf-8') as f:
        f.write('\n'.join(acc))
    
    return acc