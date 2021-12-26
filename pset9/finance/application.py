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


def get_holdings():
    """Get user stock holdings"""

    rows = db.execute("SELECT * FROM portfolios WHERE users_id = ? ORDER BY symbol ASC", session["user_id"])
    holdings = []
    stock_value = 0
    if rows:
        for row in rows:
            # Look up stock pricing
            result = lookup(row['symbol'])

            # Accumulate total stock value
            stock_value += row["shares"]*result["price"]

            # Formatting for display
            result["shares"] = row["shares"]
            result["total"] = usd(row["shares"]*result["price"])
            result["price"] = usd(result["price"])

            holdings.append(result)

    return (holdings, stock_value)


def get_cash():
    """Get user cash"""

    rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = rows[0]["cash"]

    return cash


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get user cash
    cash = get_cash()

    # Get user stock holdings
    (holdings, stock_value) = get_holdings()

    # Get stock value and total
    total = cash + stock_value

    return render_template("index.html", holdings=holdings, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Uset route reached by POST
    if request.method == "POST":

        # Get the symbol and number of shares
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Check for numeric shares
        if not shares.isnumeric():
            return apology("non-numeric input as as shares", 400)
        else:
            shares = float(shares)

        # Get the stock info
        quote = lookup(symbol)

        # Check for blank ticker
        if symbol == "":
            return apology("ticker symbol cannot be blank", 400)

        # If invalid symbol
        if quote is None:
            return apology("invalid stock symbol", 400)

        # Positive shares, at least 1
        if shares < 1:
            return apology("number of shares must be greater than 0", 400)

        # If fractional share, must be whole
        if shares % 1 != 0:
            return apology("number of shares msut be non-fractional", 400)

        # Check for sufficient funds
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        funds = rows[0]["cash"]
        price = quote["price"]
        total = quote["price"]*shares
        if funds < total:
            return apology("insufficient funds", 400)

        # Get the transaction time
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # Add transaction to transactions table
        db.execute("INSERT INTO transactions (time, users_id, symbol, type, shares, price) VALUES (?, ?, ?, ?, ?, ?)",
                   now, session["user_id"], symbol.upper(), "BUY", shares, usd(price))

        # Update portfolios table
        # Check for existing entry on symbol
        rows = db.execute("SELECT * FROM portfolios WHERE users_id = ? AND symbol LIKE ?", session["user_id"], symbol)
        # If present, update shares
        if rows:
            db.execute("UPDATE portfolios SET shares = ? WHERE users_id = ? AND symbol LIKE ?",
                       rows[0]["shares"] + shares, session["user_id"], symbol)
        # Otherwise create new entry
        else:
            print(session["user_id"])
            print(symbol)
            print(shares)
            db.execute("INSERT INTO portfolios (users_id, symbol, shares) VALUES (?, ?, ?)", session["user_id"], symbol, shares)
        # Remove rows with zero shares
        db.execute("DELETE FROM portfolios WHERE shares = 0")

        # Update cash
        db.execute("UPDATE users SET cash = ?  WHERE id = ?", funds - total, session["user_id"])

        # Redirect to index route
        return redirect("/")

    # Use route reached by GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT * FROM transactions WHERE users_id = ?", session["user_id"])

    return render_template("history.html", transactions=transactions)


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

        # If invalid symbol
        if quote is None:
            return apology("invalid stock symbol", 400)
            # return render_template("quote.html")

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
            return apology("paswords must match", 400)

        # check password length
        if len(password) < 8:
            return apology("password must be 8 characters or more in length", 400)

        # check for password numbers
        if password.isnumeric():
            return apology("password must contain at least one letter", 400)

        # check for password letters
        if password.isalpha():
            return apology("password must contain at least one number", 400)

        # check for special characters
        if password.isalnum():
            return apology("password must contain at least one special character", 400)

        # check username length
        if len(username) < 1:
            return apology("username must be 1 character or more in length", 400)

        # check if username already exists
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if rows:
            return apology("username already exists", 400)

        # hash the password
        hashed_password = generate_password_hash(password)

        # insert data into users table
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashed_password)

        # Return to login page
        return render_template("login.html")

    # Otherwise return the registration page
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Get session user_id
    # Get user portfolio
    # Check sell ticker against portfolio
    # Check if exists in portfolio
    # Check if have enough stocks to sell

    # Get user stock holdings
    (holdings, stock_value) = get_holdings()

    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Check for symbol selected
        if symbol == "":
            return apology("No stock selected", 400)

        # Check has holdings of that stock
        if symbol not in [x["symbol"] for x in holdings]:
            return apology("Must own shares of stock to sell", 400)

        # Check for numeric shares
        if not shares.isnumeric():
            return apology("non-numeric input as as shares", 400)
        else:
            shares = float(shares)

        # Check for positive integer
        if float(shares) % 1 != 0 and shares > 0:
            return apology("Shares must be non-integer", 400)

        # Check for sufficient shares
        sufficient_shares = False
        for holding in holdings:
            if holding["symbol"] == symbol:
                if holding["shares"] >= shares:
                    sufficient_shares = True
                    break
        if not sufficient_shares:
            return apology("Insufficient shares", 400)

        # Get the stock info
        quote = lookup(symbol)

        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        funds = rows[0]["cash"]
        price = quote["price"]
        total = quote["price"]*shares

        # Get the transaction time
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # Add transaction to transactions table
        db.execute("INSERT INTO transactions (time, users_id, symbol, type, shares, price) VALUES (?, ?, ?, ?, ?, ?)",
                   now, session["user_id"], symbol.upper(), "SELL", shares, usd(price))

        # Update portfolio
        # Check for existing entry on symbol
        rows = db.execute("SELECT * FROM portfolios WHERE users_id = ? AND symbol LIKE ?", session["user_id"], symbol)
        # If present, update shares
        if rows:
            db.execute("UPDATE portfolios SET shares = ? WHERE users_id = ? AND symbol LIKE ?",
                       rows[0]["shares"] - shares, session["user_id"], symbol)
        # Remove rows with zero shares
        db.execute("DELETE FROM portfolios WHERE shares = 0")

        # Update user cash
        db.execute("UPDATE users SET cash = ?  WHERE id = ?", funds + total, session["user_id"])

        return redirect("/")

    # GET route to display form to sell stock
    else:
        return render_template("sell.html", holdings=holdings)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
