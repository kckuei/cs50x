import os
from datetime import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # display a table with all current users stocks, number of shares of each, current price of each stock, and total value of each holding
    # display the user's current cash balance


    # [LEFT OFF HERE]
    # Maybe should re-implement table as TRANSACTIONS
    # Add one other table with current portfolio


    return render_template("index.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # When  GET, should display form to buy stock
    # When POST, purchase stock so long as user can afford it
    # create one or more databases here for index later

    # Uset route reached by POST
    if request.method == "POST":

        # Get stock info
        symbol = request.form.get("symbol")
        shares = float(request.form.get("shares"))
        quote = lookup(symbol)

        # If invalid symbol or empty list returned
        if not quote:
            return apology("invalid stock symbol", 403)

        # Positive shares, at least 1
        if shares < 1:
            return apology("number of shares must be greater than 0", 403)

        # Check for sufficient funds
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        funds = rows[0]["cash"]
        price = quote["price"]*shares
        if funds < price:
            return apology("insufficient funds", 403)

        # Get the transaction time
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # Add new table for keeping track of purchases
        # CREATE TABLE purchases (transac_time TEXT NOT NULL, users_id INTEGER, symbol TEXT NOT NULL, shares INTEGER, price NUMERIC);
        # CREATE UNIQUE INDEX transac_time ON purchases (transac_time)

        # Update purchase table
        db.execute("INSERT INTO purchases (transac_time, users_id, symbol, shares, price) VALUES (?, ?, ?, ?, ?)",
                    now, session["user_id"], symbol, shares, price)

        # Update cash
        db.execute("UPDATE users SET cash = ?  WHERE id = ?", funds - price, session["user_id"])

        # Render an apology without completing a purchase
        return render_template("index.html")

    # Use route reached by GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # display a table with a history of all transaction's,
    # listing row by row every buy and every sell event

    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        # Get the stock price
        quote = lookup(request.form.get("symbol"))

        return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=usd(quote["price"]))

    # GET request
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # check password matches confirmation
        if password != confirmation:
            return apology("paswords must match", 403)

        # check password length
        if len(password) < 8:
            return apology("password must be 8 characters or more in length", 403)

        # check username length
        if len(username) < 1:
            return apology("username must be 1 character or more in length", 403)

        # check if username already exists
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if rows:
            return apology("username already exists", 403)

        # hash the password
        hashed_password = generate_password_hash(password)

        # insert data into users table
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashed_password)

        # Return to login page
        return  render_template("login.html")

    # Otherwise return the registration page
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # If GET, display form to sell a stock
    # If POST, sell specified number of shares of stock, and update the user's cash

    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
