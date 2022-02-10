https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/about-readmes


Inspiration/Context:

* I'm a goetechnical engineer by training who does lots of seismic analysis/numerical modelling, so I wanted to do something relevant, but not overachieving. On that note, i thoguht it would be cool/fun to build something that cna visualize earthquakes. 
* Originally I thought about doing a 'cat book riff', with ML and backend for verifying photos. this would build on some AI courses I took previously, where we build a llamma classifier, and also some of the backend python frameworks we learned about in CS50; ultimately, this was an easier project to tackle/dive right into, and that I figure could built additional features on and learn from later in the future. Also, I didn't want to complicate the project with too many components (e.g. learning React, frameworks like Flask/Django, etc.)
* Just wanted a website with some HTML, some CSS, and some javascript, and I wanted to work with someone elses API, to just visualize data. Okay, enough about that.
* There are several public agencies/resources from which you can access earthquake data. E.g. USGS for US, and NRCAN for Canada. USGS has an API for accessing data. To my knowledge at time of writing, NRCAN doesn't it, but they have a form input, that you can scrape the return of to get the data you need (I've done this before with python selenium module).
* I did a lot of googling to just scrape something together, and then refined it gradually, as I got the skeleton pieces in. 
* I started with looking at USGS's API, and playing around with getting return values, then exploring the API object fields to see what I could plot/manipulate. It was pretty straightforward from there. 
https://earthquake.usgs.gov/earthquakes/feed/v1.0/geojson.php
* The main fields I'm using are features.geometry or features.properties
* In terms of plotting packages, there are several, JSChart, D3, Plotly. I used Plotly, because it was simple, and found some clean examples. 


How it works:

* Enter filter critiera, e.g. date range, mangitute range, limit
* The plots/meta tables should update automatically!
* Save a figure if desired?


Current to do:

* update bubble scaling to use log scale
* update to consider current time -> back 1 month
* add location filter (checkbox approach), and draw box on map
* add minimum/maximum/median/magnitudes

* API queries and callback logic
* CSS styling
* Download CSV/save image
* Plot the user location in the map
* Update time to use Date.now()
* sorting Tables
* filter buttons for magnitude?
* collapsible fields
* Javascript for DOM elements
* Function for updating plot when field update or onblick
* Update API call to consider inputs
* Add async await syntatic sugar | https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Async_await
* Responsive design
* Try hosting on a small service provider like Heroku, Digital Ocean, Github


Potential future feature updates:

* Add Canada earhtquakes by using info from NRCAN https://earthquakescanada.nrcan.gc.ca/stndon/NEDB-BNDS/bulletin-en.php
* Add more visual effects, e.g. https://www.scsn.org/ for inspiration (magnitude vs date, number of earthquakes per date)
* Add statistics / fast facts, e.g. Magnitude <2 2 3 4 5 >=6 || w/ totals... or as shown i nabove link



