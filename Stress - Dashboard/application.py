from flask import Flask, render_template, session, redirect

application = app = Flask(__name__)

@app.route('/', methods=['GET'])
def index():
    return render_template('dashboard.html')

@app.route('/dashboard/', methods=['GET'])
def dashboard():
    return render_template('dashboard.html')

@app.route('/About/', methods=['GET'])
def About():
    return render_template('About.html')

@app.route('/Analysis/', methods=['GET'])
def Analysis():
    return render_template('Analysis.html')
    
@app.route('/Doctor/', methods=['GET'])
def Doctor():
    return render_template('Doctor.html')
    
@app.route('/Patients/', methods=['GET'])
def Patients():
    return render_template('Patients.html')


if __name__ == "__main__":
    application.run(host = '0.0.0.0', debug=True)
