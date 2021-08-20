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
	except:
		print ("I am unable to connect to the database")
	cur = con.cursor()

def getData():
	cur.execute("SELECT * FROM Payload")
	data=cur.fetchall()
	return data

def update(ID,Amount):
	global cur
	global con
	cur.execute("UPDATE Payload SET Amount = Amount- {0} WHERE ID = '{1}'".format(ID,Amount))
	con.commit()
	cur.execute("SELECT * FROM Payload")
	data=cur.fetchall()
	return data

@app.route('/',methods=['GET', 'POST'])
def index():
	data=getData()
	if request.method == 'POST':
		try:
			ID = request.form["ID"]
			Amount = request.form["AM"]
			cur.execute("SELECT Amount FROM Payload WHERE ID ='{0}'".format(ID) )
			a=cur.fetchall()
			b=a[0]
			c=b[0]
			if c>0 and c-int(Amount)>0:
				data = update(ID,Amount)
			else:
				print("Cant Updata")
		except:
			pass
	return render_template('index.html', data=data)

@app.route('/worker',methods=['GET', 'POST'])
def worker():
	return render_template('worker.html')


if __name__ == '__main__':
	app.debug = True
	init_DB()
	app.run()