import json
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

timelist =[]
xAccllist=[]
yAccllist=[]
zAccllist=[]
lightlist=[]
menheraflglist=[]
heartlist=[]
Templist=[]
Humidlist=[]

InputFileName = '2020-02-01.log'
OutputGraphFileName = '20200201.png'

LinesAll = [line.rstrip() for line in open(InputFileName)]
for linesdict in LinesAll:
   msg_json = json.loads(linesdict)
   timelist.append(msg_json['time_sensor'])
   xAccllist.append(msg_json['xAccl'])
   yAccllist.append(msg_json['yAccl'])
   zAccllist.append(msg_json['zAccl'])
   lightlist.append(msg_json['light'])
   menheraflglist.append(msg_json['menheraflg'])
   heartlist.append(msg_json['heart'])
   Templist.append(msg_json['Temp'])
   Humidlist.append(msg_json['Humid'])


sensor_df = pd.DataFrame({
    'Time':   timelist
    ,'Humid': Humidlist
    ,'Temp':  Templist
    ,'Menhera': menheraflglist
    ,'heart': heartlist
    ,'light' : lightlist
    ,'xAccl' : xAccllist
    ,'yAccl' : yAccllist
    ,'zAccl' : zAccllist
})
#sensor_df['Time']
fig, axe = plt.subplots(2, 3 ,figsize=(8.0, 6.0)) # 図を2行2列に分割
# 各領域で折れ線グラフをプロット
axe[0, 0].plot(sensor_df['Time'], sensor_df['Humid'], color = 'red', marker = 'o', linestyle = '-')
axe[0, 1].plot(sensor_df['Time'], sensor_df['Temp'], color = 'green', marker = 'x', linestyle = '--')
axe[0, 2].plot(sensor_df['Time'], sensor_df['light'], color = 'yellow', marker = 'x', linestyle = '--')
axe[1, 0].plot(sensor_df['Time'], sensor_df['Menhera'], color = 'blue', marker = 's', linestyle = ':')
axe[1, 1].plot(sensor_df['Time'], sensor_df['heart'], color = 'orange', marker = 'v', linestyle = '-.')
axe[1, 2].plot(sensor_df['Time'], sensor_df['xAccl'], color = 'red', marker = 'o', linestyle = '-')
axe[1, 2].plot(sensor_df['Time'], sensor_df['yAccl'], color = 'green', marker = 'x', linestyle = '-')
axe[1, 2].plot(sensor_df['Time'], sensor_df['zAccl'], color = 'blue', marker = 'v', linestyle = '-')

# 各グラフのタイトル・x軸のラベル・y軸のラベルを設定
#axe[0, 0].set_title('Humid')
axe[0, 0].set_xlabel('Humid')
#axe[0, 0].set_ylabel('Value')
axe[0, 0].set_xticklabels([])

#axe[0, 1].set_title('Temp')
axe[0, 1].set_xlabel('Temp')
#axe[0, 1].set_ylabel('Value')
axe[0, 1].set_xticklabels([])

#axe[0, 2].set_title('light')
axe[0, 2].set_xlabel('light')
#axe[0, 2].set_ylabel('Value')
axe[0, 2].set_xticklabels([])

#axe[1, 0].set_title('Menhera')
axe[1, 0].set_xlabel('Menheraflg')
#axe[1, 0].set_ylabel('Value')
axe[1, 0].set_xticklabels([])

#axe[1, 1].set_title('heart')
axe[1, 1].set_xlabel('heart')
#axe[1, 1].set_ylabel('Value')
axe[1, 1].set_xticklabels([])

#axe[1, 2].set_title('Active')
axe[1, 2].set_xlabel('Active')
#axe[1, 2].set_ylabel('Value')
axe[1, 2].set_xticklabels([])
plt.suptitle('メンタルウォッチ  ['+timelist[0]+'～'+timelist[len(timelist)-1]+'] データ総数:'+str(len(timelist)),y=1,fontname="MS Gothic")
plt.tight_layout()
#plt.show()                              # 図の表示
plt.savefig(OutputGraphFileName)