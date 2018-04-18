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

function play_check(){
        var playpause = document.getElementById("play-pause");
        var Flag=(playpause.getAttribute("src",2)=="images/play.png");
        playpause.src=Flag?"images/pause.png":"images/play.png";
        if(Flag)
                getData('cgi-bin/music.cgi', 'POST', 'msg=play');
        else
                getData('cgi-bin/music.cgi', 'POST', 'msg=pause');
}

function sound_check(){
        var soundflag = document.getElementById("sound-quite");
        var Flag=(soundflag.getAttribute("src",2)=="images/sound.png");
        soundflag.src=Flag?"images/quiet.png":"images/sound.png";
		if(Flag)
                getData('cgi-bin/music.cgi', 'POST', 'msg=sound');
        else
                getData('cgi-bin/music.cgi', 'POST', 'msg=quiet');
}

function next_check(){
	 getData('cgi-bin/music.cgi', 'POST', 'msg=next');
}

function prev_check(){
	 getData('cgi-bin/music.cgi', 'POST', 'msg=prev');
}

function music_black(){
	history.back();
}
