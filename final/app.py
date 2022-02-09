from flask import Flask, redirect, render_template, request, session
#from flask_session import Session
from tempfile import mkdtemp


# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True
app.config

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Configure session to use filesystem (instead of signed cookies)
# app.config["SESSION_FILE_DIR"] = mkdtemp()
# app.config["SESSION_PERMANENT"] = False
# app.config["SESSION_TYPE"] = "filesystem"
#Session(app)

# # Configure CS50 Library to use SQLite database
# db = SQL("sqlite:///finance.db")

# # Make sure API key is set
# if not os.environ.get("API_KEY"):
#     raise RuntimeError("API_KEY not set")


# @app.route("/")
# @login_required
# def index():
#     """Show portfolio of stocks"""

#     # Get user cash
#     cash = get_cash()

#     # Get user stock holdings
#     (holdings, stock_value) = get_holdings()

#     # Get stock value and total
#     total = cash + stock_value

#     return render_template("index.html", holdings=holdings, cash=usd(cash), total=usd(total))


# @app.route("/login", methods=["GET", "POST"])
# def login():
#     """Log user in"""

#     # Forget any user_id
#     session.clear()

#     # User reached route via POST (as by submitting a form via POST)
#     if request.method == "POST":

#         # Ensure username was submitted
#         if not request.form.get("username"):
#             return apology("must provide username", 403)

#         # Ensure password was submitted
#         elif not request.form.get("password"):
#             return apology("must provide password", 403)

#         # Query database for username
#         rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

#         # Ensure username exists and password is correct
#         if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
#             return apology("invalid username and/or password", 403)

#         # Remember which user has logged in
#         session["user_id"] = rows[0]["id"]

#         # Redirect user to home page
#         return redirect("/")

#     # User reached route via GET (as by clicking a link or via redirect)
#     else:
#         return render_template("login.html")

@app.route("/")
def index():
    name = request.args.get("name")
    return render_template("index.html", name=name)