// initialize html element pointers
let minMagElem = document.getElementById("minMagBox")
let maxMagElem = document.getElementById("maxMagBox")
let minDateElem = document.getElementById("minDateBox")
let maxDateElem = document.getElementById("maxDateBox") 
let limitElem = document.getElementById("limitBox") 

// initialize querySettings
const querySettings = {
    minMag : Math.max(0, minMagElem.value), 
    maxMag : Math.min(10, maxMagElem.value),
    minDate : minDateElem.value, 
    maxDate : maxDateElem.value,
    limit : Math.min(20000, limitElem.value),
}

// add event callbacks
minMagElem.addEventListener('change', (event) => {
    querySettings.minMag = Math.max(0, minMagElem.value);
    fetchApiDataAndRender()
});

maxMagElem.addEventListener('change', (event) => {
    querySettings.maxMag = Math.min(10, maxMagElem.value);
    fetchApiDataAndRender()
});

minDateElem.addEventListener('change', (event) => {
    querySettings.minDate = minDateElem.value;
    fetchApiDataAndRender()
});

maxDateElem.addEventListener('change', (event) => {
    querySettings.maxDate = maxDateElem.value;
    fetchApiDataAndRender()
});

limitElem.addEventListener('change', (event) => {
    querySettings.limit = limitElem.value;
    fetchApiDataAndRender()
});



// storing api call results in this object to iterate /reference later
let data;

// plot constants
const MARKERSCALEFAC = 10;
const MAGCOLORS = ['#ffffb2','#fed976','#feb24c','#fd8d3c','#f03b20','#bd0026']


function fetchApiDataAndRender() {

    // initialize plot payload object
    let plotPayload = {
        place: [], 
        time: [],
        mag: [], 
        lon: [],
        lat: [],
        size: [],
        color: [],
        urls: [],
    }

    // construct API query
    let baseURL = "https://earthquake.usgs.gov/fdsnws/event/1/query?format=geojson"
    let dateQuery = `&starttime=${querySettings.minDate}&endtime=${querySettings.maxDate}`
    let magQuery = `&minmagnitude=${querySettings.minMag}&maxmagnitude=${querySettings.maxMag}`
    let limitQuery = `&limit=${querySettings.limit}`
    let fullApiQuery = baseURL + dateQuery + magQuery + limitQuery

    // call the USGS API
    fetch(fullApiQuery)
    .then(response => response.json())
    .then(obj => data = obj)
    // .then(() => console.log(data))
    .then(() => {

                  // populate the fields we need for plotting
                  let innterHtmlStringFacts = "<tr><th>Magnitude</th><th>< 2</th><th>2</th><th>3</th><th>4</th><th>5</th><th>≥ 6</th></tr>"
                  let innerHtmlStringEqkEvents = "<tr><th>No.</th><th>Date (YYYY-MM-DD)</th><th>Time (HH-MM-SS)</th><th>Magnitude</th><th>Longitude</th><th>Lattitude</th><th>Place</th><th>URL</th></tr>"
                  for (let i = 0; i < data.features.length; i++) {
                      plotPayload.place.push(data.features[i].properties.place);
                      plotPayload.time.push(new Date(data.features[i].properties.time));
                      plotPayload.mag.push(data.features[i].properties.mag);
                      plotPayload.lon.push(data.features[i].geometry.coordinates[0]);
                      plotPayload.lat.push(data.features[i].geometry.coordinates[1]);
                      plotPayload.urls.push(data.features[i].properties.url)
                      plotPayload.size.push(data.features[i].properties.mag * MARKERSCALEFAC);
                      plotPayload.color.push(assignColor(data.features[i].properties.mag))
                      
                      innerHtmlStringEqkEvents += `<tr>
                      <td>${i}</td>
                      <td>${plotPayload.time[i].getFullYear()}-${plotPayload.time[i].getMonth()+1}-${plotPayload.time[i].getDate()}</td>
                      <td>${plotPayload.time[i].getHours()}-${plotPayload.time[i].getMinutes()+1}-${plotPayload.time[i].getSeconds()}</td>
                      <td>${Math.round(plotPayload.mag[i] * 100)/100}</td>
                      <td>${Math.round(plotPayload.lon[i] * 100)/100}</td>
                      <td>${Math.round(plotPayload.lat[i] * 100)/100}</td>
                      <td class=tdPlace>${plotPayload.place[i]}</td>
                      <td><a href=${plotPayload.urls[i]} target="_blank">Main</a></td>
                      </tr>`;
                  }

                  // create summary table of statistics
                  document.getElementById('statisticsTable').innerHTML = innterHtmlStringFacts;

                  // create summary table of events
                  document.getElementById('entriesTable').innerHTML = innerHtmlStringEqkEvents;
  
                  // render charts
                  renderMap(plotPayload)
                  renderTimeSeriesLine(plotPayload)
                }
    );
}



function assignColor(magnitude) {
    // magnitude colormap for map

    if (magnitude < 2) {
        return MAGCOLORS[0]
    } else if (magnitude < 3) {
        return MAGCOLORS[1]
    } else if (magnitude < 4) {
        return MAGCOLORS[2]
    } else if (magnitude < 5) {
        return MAGCOLORS[3]
    } else if (magnitude < 6) {
        return MAGCOLORS[4]
    } else {
        return MAGCOLORS[5]
    }
}


function renderMap(plotPayload) {

    var data = [{
        type: 'scattergeo',
        mode: 'markers+text',
        text: plotPayload.text, 
        lon: plotPayload.lon, 
        lat: plotPayload.lat, 
        marker: {
            size: plotPayload.size,
            color: plotPayload.color,
            line: {
                width: 1
            }
        },

        name: 'Earthquakes',
        textposition: [
            'top right', 'top left', 'top center', 'bottom right', 'top right',
            'top left', 'bottom right', 'bottom left', 'top right', 'top right'
        ],
    }];
    
    var layout = {
        autosize: false, 
        width: 1000, 
        height: 800,
        margin: {
            autoexpand : true,
            b: 0,
            t: 0, 
            l: 0, 
            r: 0,
        },
        title: '',
        font: {
            family: 'Roboto, serif',
            size: 6
        },
        titlefont: {
            size: 16
        },
        geo: {
            scope: 'north america',
            resolution: 50,
            lonaxis: {
                'range': [-130, -55]
            },
            lataxis: {
                'range': [40, 70]
            },
            showrivers: true,
            rivercolor: '#fff',
            showlakes: true,
            lakecolor: '#fff',
            showland: true,
            landcolor: '#969696',
            countrycolor: '#d3d3d3',
            countrywidth: 1.5,
            subunitcolor: '#d3d3d3'
        }
    };
    
    Plotly.newPlot('magMapChart', data, layout);

}


function renderTimeSeriesLine(plotPayload) {

    d3.csv("https://raw.githubusercontent.com/plotly/datasets/master/finance-charts-apple.csv", function(err, rows){

        function unpack(rows, key) {
        return rows.map(function(row) { return row[key]; });
      }
      
      
      var trace1 = {
        type: "scatter",
        mode: "lines",
        name: 'AAPL High',
        x: unpack(rows, 'Date'),
        y: unpack(rows, 'AAPL.High'),
        line: {color: '#17BECF'}
      }
      
      var trace2 = {
        type: "scatter",
        mode: "markers",
        name: 'AAPL Low',
        x: unpack(rows, 'Date'),
        y: unpack(rows, 'AAPL.Low'),
        line: {color: '#7F7F7F'}
      }
      
      var data = [trace1,trace2];
      
      var layout = {
        title: '',
        xaxis: {
          autorange: true,
          range: ['2015-02-17', '2017-02-16'],
          rangeselector: {buttons: [
              {
                count: 1,
                label: '1m',
                step: 'month',
                stepmode: 'backward'
              },
              {
                count: 6,
                label: '6m',
                step: 'month',
                stepmode: 'backward'
              },
              {step: 'all'}
            ]},
          rangeslider: {range: ['2015-02-17', '2017-02-16']},
          type: 'date'
        },
        yaxis: {
          autorange: true,
          range: [86.8700008333, 138.870004167],
          type: 'linear'
        }
      };
      
      Plotly.newPlot('magTimeSeriesLineChart', data, layout);
      })
      
    
}


function renderTimeSeriesScatter() {

}




fetchApiDataAndRender()