// be kinda cool to combine canadian API with US
// https://earthquakescanada.nrcan.gc.ca/stndon/NEDB-BNDS/bulletin-en.php?display=1&endtime=2022-02-09T23%3A59%3A59&eventtype=L&latitude=63.13&limit=4001&longitude=-90.34&maxdepth=1000&maxlatitude=90&maxlongitude=180&maxmagnitude=10&maxradius=45.04504504504504&mindepth=-5&minlatitude=-90&minlongitude=-180&minmagnitude=-5&minradius=0&onlyfelt=0&starttime=2022-01-09T00%3A00%3A00
// add a table of summary statistics and earthquake metatable as well
// Magnitude <2 2 3 4 5 >=6 || w/ totals...

// Need a function to update plot when field updated, or onclick

// Need get javascript DOM elements


const querySettings = {
    minMag : Math.max(0,document.getElementById("minMagBox").value), 
    maxMag : Math.min(10,document.getElementById("maxMagBox").value)

}
// const minMag = Math.max(0,document.getElementById("minMagBox"))
// const maxMag = Math.min(10,document.getElementById("maxMagBox"))
// obj.features[0].properties


// storing api call results in this object to iterate /referenc elater
let obj;

// maybe can encapsulate these into a delivery object later
let text = [];
let mag = [];
let lon = [];
let lat = [];
let size = [];
let color = [];

const markerScaleFac = 10;
const magColors = ['#ffffb2','#fed976','#feb24c','#fd8d3c','#f03b20','#bd0026']

function fetchApiDataAndRender() {
    let apiQuery = `https://earthquake.usgs.gov/fdsnws/event/1/query?format=geojson&starttime=2014-01-01&endtime=2014-01-02&minmagnitude=${querySettings.minMag}`
    fetch(apiQuery)
    .then(response => response.json())
    .then(data => obj = data)
    .then(() => console.log(obj))
    .then( () => {

                  // populate the fields we need for plotting
                  let innerHtmlString = "<tr><th>No.</th><th>Magnitude</th><th>Longitude</th><th>Lattitude</th><th>Place</th></tr>"
                  for (let i = 0; i < obj.features.length; i++) {
                      text.push(obj.features[i].properties.place);
                      mag.push(obj.features[i].properties.mag);
                      lon.push(obj.features[i].geometry.coordinates[0]);
                      lat.push(obj.features[i].geometry.coordinates[1]);
                      size.push(obj.features[i].properties.mag * markerScaleFac);
                      color.push(assignColor(obj.features[i].properties.mag))
                      
                      innerHtmlString += `<tr>
                      <td>${i}</td>
                      <td>${mag[i]}</td>
                      <td>${lon[i]}</td>
                      <td>${lat[i]}</td>
                      <td>${text[i]}</td>
                      </tr>`;
                  }

                  // create a table for showing results

                  document.getElementById('entriesTable').innerHTML = innerHtmlString;

  
                  renderMap()
                }
    );
}


function assignColor(magnitude) {
    if (magnitude < 2) {
        return magColors[0]
    } else if (magnitude < 3) {
        return magColors[1]
    } else if (magnitude < 4) {
        return magColors[2]
    } else if (magnitude < 5) {
        return magColors[3]
    } else if (magnitude < 6) {
        return magColors[4]
    } else {
        return magColors[5]
    }
}

function renderMap() {

    var data = [{
        type: 'scattergeo',
        mode: 'markers+text',
        text: text, 
        lon: lon, 
        lat: lat, 
        marker: {
            size: size,
            color: color,
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
        height: 1000,
        title: 'Earthquakes',
        font: {
            family: 'Droid Serif, serif',
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
    
    Plotly.newPlot('myDiv', data, layout);

}


fetchApiDataAndRender()