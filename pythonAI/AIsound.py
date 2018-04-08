import socket
import string
import sys
#from aip import AipSpeech

APP_ID = '10920915'
API_KEY = 'xG7KMCpv0x9stVVbk1EdWSSb'
SECRET_KEY = '4dbface55ed062109b3727a97fba24e0'


# 初始化AipSpeech对象
#aipSpeech = AipSpeech(APP_ID, API_KEY, SECRET_KEY)

# 读取文件
def get_file_content(filePath):
    with open(filePath, "rb") as fp:
        return fp.read()

def music_control(Apiresult):
    if Apiresult.find('暂停')>= 0:
       msg = '0'
    elif Apiresult.find('播放')>= 0:
       msg = '1'
    elif Apiresult.find('下一首')>= 0:
       msg = '2'
    elif Apiresult.find('上一首')>= 0:
       msg = '3'
    elif Apiresult.find('停止')>= 0:
       msg = '4'
    else:
       msg = '5'
    return msg

def play_mode(Apiresult):
    if Apiresult.find('单曲播放')>= 0:
       msg = '0'
    elif Apiresult.find('单曲循环')>= 0:
       msg = '1'
    elif Apiresult.find('循环播放')>= 0:
       msg = '2'
    elif Apiresult.find('随机')>= 0:
       msg = '3'
    else:
       msg = '2'
    return msg


def socket_creat(Apiresult):
    #创建socket对象
    ClientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = '127.0.0.1'
    port = 6666
    #连接服务，指定主机和端口
    ClientSocket.connect((host, port))
    #播放控制
    msg = music_control(Apiresult)
    #播放模式
    mod = play_mode(Apiresult)
    msg = msg + mod
    ClientSocket.send(msg.encode('utf-8'))
    ClientSocket.close()

# 识别本地文件
#目前支持的格式较少，原始 PCM 的录音参数必须符合 8k/16k 采样率、16bit 位深、单声道，支持的格式有：pcm（不压缩）、wav（不压缩，pcm编码）、amr（压缩格式）。
#def bai_request():
	#result = aipSpeech.asr(get_file_content('./record.wav'), 'wav', 16000, {
    #		'lan': 'zh',
	#})
	#Apiresult = result['result'][0]
	#socket_creat(Apiresult)

Apiresult = '循环播放'
socket_creat(Apiresult)




