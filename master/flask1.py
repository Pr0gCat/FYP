from datetime import time
from flask import Flask, request, render_template, redirect, url_for
import os.path
import sqlite3

app = Flask(__name__)

Database = "data.db"
global cur
global con

def init_DB():
	global con
	global cur 
	BASE_DIR = os.path.dirname(os.path.abspath(__file__))
	db_path = os.path.join(BASE_DIR, "data.db")
	try:
		con = sqlite3.connect(db_path, check_same_thread=False)
		print("connected")
	except:
		print ("I am unable to connect to the database")
	cur = con.cursor()

def getData():
	cur.execute("SELECT * FROM Payload")
	data=cur.fetchall()
	return data

def UPDATE(ID,AM):
	global cur
	global con
	cur.execute("SELECT Amount FROM Payload WHERE ID ='"+ID+"'" )
	a=cur.fetchall()
	b=a[0]
	c=b[0]
	if c>0 and c-int(AM)>0:
		cur.execute("UPDATE Payload SET Amount = Amount-" + AM +" WHERE ID = '"+ID+"'")
		con.commit()
	else:
		print("Cant Updata")

	cur.execute("SELECT * FROM Payload")
	data=cur.fetchall()
	return data

@app.route('/',methods=['GET', 'POST'])
def index():
	data=getData()
	if request.method == 'POST':
		try:
			ID = request.form["ID"]
			AM = request.form["AM"]
			data = UPDATE(ID,AM)
		except:
			pass
	return render_template('index.html', data=data)

@app.route('/test',methods=['POST','GET'])
def test():
	return render_template('test.html')


if __name__ == '__main__':
	app.debug = True
	init_DB()
	app.run()