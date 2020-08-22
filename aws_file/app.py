
from datetime import datetime
import json
from flask import Flask, request, jsonify
app = Flask(__name__)
 
@app.route('/command', methods = ['POST'])
def command():
    c = request.get_data()#json 데이터를 받아옴
    print(c)
    f = open('./command.txt','w')
    f.write(c)
    f.close()

    return jsonify(c)# 받아온 데이터를 다시 전송
 
@app.route('/update', methods = ['GET'])
def update():
    
    _uuid = request.args["uuid"]
    _distance = request.args["distance"]

    
    if int(_distance) < 1000 : 

        f = open('./log.txt','a')
        
        log = datetime.now().strftime("%m/%d/%Y,%H:%M:%S") + '   ' + _uuid +  '   ' + _distance + '\n'
        
        f.write(log)
        f.close()

    f = open('./command.txt','r')
    command = f.readline()
    f.close()


    return command
 



if __name__ == '__main__':
    app.run(host='0.0.0.0', port = 8000)