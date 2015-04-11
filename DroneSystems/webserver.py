from flask import Flask
from flask import request
from flask import jsonify
from random import randint
import json

app = Flask(__name__)
playerPos = [50,50]
robotsPos = []
scrapPos = []
curlevel = 1
winstatus = 0

def init():
	global playerPos
	global robotsPos
	global winstatus
	
	robotsPos.clear()
	scrapPos.clear()
	winstatus = 0
	
	
	playerPos = [50,50]
	print(curlevel)
	for x in range (0,10*curlevel):
		robotsPos.append([randint(0,99),randint(0,99)])
	checkCrash()
	winstatus = checkStatus()
	return len(robotsPos)
		
@app.route("/", methods=['GET'])
def index():
	return app.send_static_file('index.html')

def teleport():
	global playerPos
	while(True):
		validPos = True
		xtel = randint(0,99)
		ytel = randint(0,99)
		for robot in robotsPos:
			if(robot[0] == xtel and robot[1] == ytel):
				validPos = False
		for scrap in scrapPos:
			if(scrap[0] == xtel and scrap[1] == ytel):
				validPos = False
		if(validPos):
			playerPos[0] = xtel
			playerPos[1] = ytel
			return
			
@app.route("/move", methods=['GET','POST'])
def move():
	global playerPos
	global winstatus
	
	direction = int(request.form['direction'])
	if direction == 0:
		playerPos[0] -=1
		playerPos[1] -=1
	elif direction == 1:
		playerPos[0] -=1
	elif direction == 2:
		playerPos[0] -=1
	elif direction == 3:
		playerPos[1] -=1
	elif direction == 4:
		teleport()
	elif direction == 5:
		playerPos[1] +=1
	elif direction == 6:
		playerPos[0] +=1
		playerPos[1] -=1
	elif direction == 7:
		playerPos[0] +=1
	elif direction == 8:
		playerPos[0] +=1
		playerPos[1] +=1
	robotMove()
	checkCrash()
	winstatus = checkStatus()
	#return json.dumps({level: curlevel, winstatus: winstatus, player: playerPos, robots: robotsPos, scrap: scrapPos})
	return jsonify(level=curlevel, winstatus=winstatus, player=playerPos, robots=robotsPos, scrap=scrapPos)
	
@app.route("/restart", methods=['POST'])
def restart():
	global curlevel
	curlevel = int(request.form['level'])
	init()
	#return json.dumps({level: curlevel, winstatus: winstatus, player: playerPos, robots: robotsPos, scrap: scrapPos})
	return jsonify(level=curlevel, winstatus=winstatus, player=playerPos, robots=robotsPos, scrap=scrapPos)
	
def checkStatus():
	if len(robotsPos) == 0:
		return 1
	for robot in robotsPos:
		if robot[0] == playerPos[0] and robot[1] == playerPos[1]:
			return 2
	for scrap in scrapPos:
		if scrap[0] == playerPos[0] and scrap[1] == playerPos[1]:
			return 2
	return 0

def robotMove():
	global robotsPos
	for robot in robotsPos:
		if playerPos[0] > robot[0]:
			robot[0]+=1
		elif playerPos[0] < robot[0]:
			robot[0]-=1
		
		if playerPos[1] > robot[1]:
			robot[1]+=1
		elif playerPos[1] < robot[1]:
			robot[1]-=1

def checkCrash():
	global robotsPos
	global scrapPos
	crashed = []
	
	for x in range(0,len(robotsPos)):
		for y in range(0,len(scrapPos)):
			if(robotsPos[x][0] == scrapPos[y][0] and robotsPos[x][1] == scrapPos[y][1]):
				crashed.append(robotsPos[x]);
	
	for x in range(0,len(robotsPos)):
		for y in range(x+1,len(robotsPos)):
			if(robotsPos[x][0] == robotsPos[y][0] and robotsPos[x][1] == robotsPos[y][1]):
				crashed.append(robotsPos[x])
				crashed.append(robotsPos[y])

	for crash in crashed:
		robotsPos.remove(crash)
		if crash not in scrapPos:
			scrapPos.append(crash)
			
if __name__ == "__main__":
	init()
	app.run(debug=True)