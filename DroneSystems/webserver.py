from flask import Flask
from flask import request
from random import randint
app = Flask(__name__)
playerPos = []
robotsPos = []
scrapPos = []
curlevel = 1
winstatus = 0;

def init():
	playerPos = [50,50]

	for x in range (0,10*curlevel):
		robotsPos.append([randint(0,99),randint(0,99)])
	return robotsPos[9]
	checkCrash()
	winstatus = checkStatus()
	return len(robotsPos)
		
@app.route("/", methods=['GET'])

def index():
	return str(init())

@app.route("/move", methods=['GET','POST'])
def move():
	direction = request.form['direction']
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
		playerPos[1] -=1 #TODO: teleport
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
	return "Hello World!"

@app.route("/restart", methods=['POST'])
def restart():
	curlevel = request.form['level']
	init()
	
	
def checkStatus():
	if len(robotsPos) == 0:
		return 1;
	for robot in robotsPos:
		if robot[0] == playerPos[0] and robot[1] == playerPos[1]:
			return 2;
	return 0;

def robotMove():
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
	crashed = []
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
	app.run(debug=True)