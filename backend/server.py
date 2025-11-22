from flask import Flask, jsonify, send_from_directory, request, make_response
from flask_cors import CORS
import sqlite3
import os


app = Flask(__name__)

# Detect environment
ENV = os.environ.get("FLASK_ENV", "production").lower()

if ENV == "development":
    # Allow localhost, 127.0.0.1, and any LAN device
    ALLOWED_ORIGINS = [
        "http://localhost:3000",
        "http://127.0.0.1:3000",
        "http://10.0.0.*"
    ]
else:
    # Production: allow whatever is set in env
    env_origins = os.environ.get("ALLOWED_ORIGINS", "")
    if env_origins:
        ALLOWED_ORIGINS = env_origins.split(",")

CORS(
    app,
    resources={r"/backend/*": {"origins": ALLOWED_ORIGINS}},
    supports_credentials=True,
    allow_headers=["Content-Type", "Authorization"],
    methods=["GET", "POST", "PUT", "DELETE", "OPTIONS"]
)

DB_PATH = os.environ.get("DB_PATH", "/data/robot_logger.db")

def get_db_connection():
    """Helper function to connect to the SQLite database."""
    conn = sqlite3.connect(DB_PATH, check_same_thread=False)
    conn.row_factory = sqlite3.Row  # so rows can be converted to dicts
    return conn

@app.route("/backend/users")
def get_users():
    try:
        conn = get_db_connection()
        cur = conn.cursor()
        cur.execute("SELECT * FROM users")
        rows = [dict(row) for row in cur.fetchall()]
        conn.close()
        return jsonify(rows)
    except Exception as e:
        return jsonify({"error": str(e)}), 500
    
@app.route("/backend/robots")
def get_robots():
    try:
        conn = get_db_connection()
        cur = conn.cursor()
        cur.execute("SELECT * FROM robots")
        rows = [dict(row) for row in cur.fetchall()]
        conn.close()
        return jsonify(rows)
    except Exception as e:
        return jsonify({"error": str(e)}), 500
    
@app.route("/backend/logs")
def get_logs():
    try:
        conn = get_db_connection()
        cur = conn.cursor()
        cur.execute("SELECT * FROM logs")
        rows = [dict(row) for row in cur.fetchall()]
        conn.close()
        return jsonify(rows)
    except Exception as e:
        return jsonify({"error": str(e)}), 500
    
@app.route("/backend/notes")
def get_notes():
    try:
        conn = get_db_connection()
        cur = conn.cursor()
        cur.execute("SELECT * FROM notes")
        rows = [dict(row) for row in cur.fetchall()]
        conn.close()
        return jsonify(rows)
    except Exception as e:
        return jsonify({"error": str(e)}), 500


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)