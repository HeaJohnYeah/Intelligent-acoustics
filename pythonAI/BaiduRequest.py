from aip import AipSpeech

APP_ID = '10920915'
API_KEY = 'xG7KMCpv0x9stVVbk1EdWSSb'
SECRET_KEY = '4dbface55ed062109b3727a97fba24e0'


# 初始化AipSpeech对象
aipSpeech = AipSpeech(APP_ID, API_KEY, SECRET_KEY)
 
# 读取文件
def get_file_content(filePath):
    with open(filePath, "rb") as fp:
        return fp.read()
 
# 识别本地文件
#目前支持的格式较少，原始 PCM 的录音参数必须符合 8k/16k 采样率、16bit 位深、单声道，支持的格式有：pcm（不压缩）、wav（不压缩，pcm编码）、amr（压缩格式）。
def bai_request():
	result = aipSpeech.asr(get_file_content('./001.amr'), 'amr', 8000, {
    		'lan': 'zh',
	})
	print (result['result'][0])



