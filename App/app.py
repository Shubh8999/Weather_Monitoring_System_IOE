from flask import Flask, render_template, jsonify
import json
import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import smtplib
from email.message import EmailMessage

app = Flask(__name__)
rainStartEmailSent = False
rainStopEmailSent = False
rainProbability = False

# SMTP server configuration
SMTP_SERVER = 'smtp.office365.com'
SMTP_PORT = 587  # for TLS
SMTP_USERNAME = 'sdgforslums@outlook.com'
SMTP_PASSWORD = 'LBSwinner'
app = Flask(__name__)

# def customCallback(client, userdata, message):
#     print("Callback came...")
#     print("Topic: " + message.topic)
#     payload = message.payload.decode()
#     print("Message: " + payload)
    
#     # Save the received data to a JSON file
#     save_to_json(payload)

# def save_to_json(data):
#     try:
#         with open('../received_data.json', 'a') as json_file:
#             json.dump(data, json_file)
#             json_file.write('\n')
#         print("Data saved to received_data.json")
#     except Exception as e:
#         print("Error saving data to JSON file:", str(e))

def send_rain_start_notification(subject,message):
  

    msg = EmailMessage()
    msg['Subject'] = subject
    msg['From'] = SMTP_USERNAME
    msg['To'] = 'shubhamkamodkar262@gmail.com'  # Replace with the recipient's email address
    msg.set_content(f'''
        <!DOCTYPE html>
        <html>
            <body>
                <div style="background-color:#eee;padding:10px 20px;">
                    <h2 style="font-family:Georgia, 'Times New Roman', Times, serif;color#454349;">{subject}</h2>
                </div>
                <div style="padding:20px 0px">
                    <div>
                        
                        <div>
                            <p>
                                Dear User,<br/><br/>
                                {message}<br/><br/>
                                Best regards,<br/>
                                WeatherBuddy<br/>
                            </p>
                        </div>
                    </div>
                </div>
            </body>
        </html>
    ''', subtype='html')

    try:
        smtp = smtplib.SMTP(SMTP_SERVER, SMTP_PORT)
        smtp.starttls()
        smtp.login(SMTP_USERNAME, SMTP_PASSWORD)
        smtp.send_message(msg)
        smtp.quit()
        print("Email notification sent successfully")
    except Exception as e:
        print("Error sending email notification:", str(e))


def send_rain_stop_notification(subject,message):

    msg = EmailMessage()
    msg['Subject'] = subject
    msg['From'] = SMTP_USERNAME
    msg['To'] = 'shubhamkamodkar262@gmail.com'  # Replace with the recipient's email address
    msg.set_content(f'''
        <!DOCTYPE html>
        <html>
            <body>
                <div style="background-color:#eee;padding:10px 20px;">
                    <h2 style="font-family:Georgia, 'Times New Roman', Times, serif;color#454349;">{subject}</h2>
                </div>
                <div style="padding:20px 0px">
                    <div>
                        
                        <div>
                            <p>
                                Dear User,<br/><br/>
                                {message}<br/><br/>
                                Best regards,<br/>
                                WeatherBuddy<br/>
                            </p>
                        </div>
                    </div>
                </div>
            </body>
        </html>
    ''', subtype='html')

    try:
        smtp = smtplib.SMTP(SMTP_SERVER, SMTP_PORT)
        smtp.starttls()
        smtp.login(SMTP_USERNAME, SMTP_PASSWORD)
        smtp.send_message(msg)
        smtp.quit()
        print("Email notification sent successfully")
    except Exception as e:
        print("Error sending email notification:", str(e))


def customCallback(client, userdata, message):
    print("Callback came...")
    print("Topic: " + message.topic)
    payload = message.payload.decode()
    print("Message: " + payload)
    
    # Save the received data to a JSON file
    save_to_json(payload)

def save_to_json(data):
    try:
        with open('../received_data.json', 'a') as json_file:
            json.dump(data, json_file)
            json_file.write('\n')
        print("Data saved to received_data.json")
    except Exception as e:
        print("Error saving data to JSON file:", str(e))

@app.route('/')
def index():
    data_list = []
    myMQTTClient = AWSIoTMQTTClient("device")
    myMQTTClient.configureEndpoint("a1x4rget4tmruj-ats.iot.eu-west-1.amazonaws.com", 8883)

    myMQTTClient.configureCredentials("../AmazonRootCA1.pem", "../Private.key", "../DeviceCertificate.crt")

    myMQTTClient.connect()
    print("Client Connected")

    # Subscribe to the analytics_topic with QoS 1
    myMQTTClient.subscribe("esp32/pub", 1, customCallback)
    print('Waiting for the callback. Press Enter to continue...')



    
    return render_template('index.html')

@app.route('/get_data')
def get_data():
    # Read data from the JSON file
    global rainStartEmailSent,rainStopEmailSent, rainProbability
    data_list = []
    with open('../received_data.json', 'r') as json_file:
        for line in json_file:
            data_list.append(json.loads(line))
    send_list = []
    for data in data_list:
        send_list.append(json.loads(data))
    if send_list[len(send_list)-1]['rain_status'] == 1 and not rainStartEmailSent:
        rainStartEmailSent=True
        rainStopEmailSent=False
        print("Rain status changed to 1, Email sent")
        send_rain_start_notification(subject='Rain Alert: Precipitation Detected',message=' We wanted to inform you that our system has detected the start of rain in your area.</br>Please take necessary precautions and consider adjusting your plans accordingly, as road conditions may change, and outdoor activities may be affected.')
    elif send_list[len(send_list)-1]['rain_status'] == 0 and not rainStopEmailSent:
        rainStopEmailSent=True
        rainStartEmailSent=False
        print("Rain status changed to 0, Email Sent")
        send_rain_stop_notification(subject='Rain Alert Update: Precipitation Has Stopped',message='We hope this message finds you well. We would like to provide you with an update from our system: the rain that was previously detected has now ceased. </br> The rain has come to an end, and conditions are expected to improve. It\'s safe to resume your regular activities and travel plans without the inconvenience of wet weather.</br>As always, we are here to keep you informed about changes in weather conditions. If you have any questions or need further information, please do not hesitate to reach out to us.</br>Wishing you a pleasant day ahead')
    elif send_list[len(send_list)-1]['humidity'] > 95 and not rainProbability:
        rainProbability=True
        print("High Humidity rain probability, Email Sent")
        send_rain_stop_notification(subject='High Humidity',message='Our system has detected a significant increase in humidity levels, which often indicates an elevated chance of rain. While it might be dry at the moment, it\'s prudent to be prepared for potential rain showers.</br> We recommend considering appropriate precautions, such as carrying an umbrella or planning your outdoor activities accordingly. Staying informed about changing weather conditions is crucial for your safety and comfort.')
    elif send_list[len(send_list)-1]['humidity'] < 95 :
        rainProbability=False

    return send_list

    



if __name__ == '__main__':
    app.run(debug=True)
