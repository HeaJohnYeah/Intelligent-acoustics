function getData(url, type, data, success, error) {
        $.ajax({
                url: url,
                type: type, //GET或者post
                data: data,
                success: function(data, a, b) {
                        //返回的数据
                },
                error:function(e){
                }
        })
}

function shutdown_check(){
	getData('cgi-bin/aitalk.cgi', 'POST', 'msg=shutdown');
}

function record_check(){
	getData('cgi-bin/aitalk.cgi', 'POST', 'msg=record');
}

function aiback_check(){
	history.back();
}