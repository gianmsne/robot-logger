from flask import Flask, jsonify
from flask_cors import CORS
import sqlite3
import os


app = Flask(__name__)
CORS(app)

# Use absolute path based on this file location
BASE_DIR = os.path.dirname(__file__)
DB_PATH = os.path.join(BASE_DIR, "..", "database", "robot_logger.db")

def get_db_connection():
    """Helper function to connect to the SQLite database."""
    conn = sqlite3.connect(os.path.normpath(DB_PATH))
    conn.row_factory = sqlite3.Row  # so rows can be converted to dicts
    return conn

@app.route("/api/users")
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


if __name__ == "__main__":
    app.run(debug=True)
