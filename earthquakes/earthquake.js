// Need a function to update plot when field updated, or onclick

// Need get javascript DOM elements

const minMagVal = document.getElementByID("minMagBox")
const maxMagVal = document.getElementByID("maxMagBox")


let obj;

fetch("https://earthquake.usgs.gov/fdsnws/event/1/query?format=geojson&starttime=2014-01-01&endtime=2014-01-02")
  .then(response => response.json())
  .then(data => obj = data)
  .then(() => console.log(obj))
  .then( () => {
				for (let i = 0; i < obj.features.length; i++) {
					console.log(obj.features[i].id);
					console.log(obj.features[i].detail);
				}

                render()
			  }
  );






function render() {


    var data = [{
        type: 'scattergeo',
        mode: 'markers+text',
        text: [
            'Montreal', 'Toronto', 'Vancouver', 'Calgary', 'Edmonton',
            'Ottawa', 'Halifax', 'Victoria', 'Winnepeg', 'Regina'
        ],
        lon: [
            -73.57, -79.24, -123.06, -114.1, -113.28,
            -75.43, -63.57, -123.21, -97.13, -104.6
        ],
        lat: [
            45.5, 43.4, 49.13, 51.1, 53.34, 45.24,
            44.64, 48.25, 49.89, 50.45
        ],
        marker: {
            size: 7,
            color: [
                '#bebada', '#fdb462', '#fb8072', '#d9d9d9', '#bc80bd',
                '#b3de69', '#8dd3c7', '#80b1d3', '#fccde5', '#ffffb3'
            ],
            line: {
                width: 1
            }
        },
        name: 'Canadian cities',
        textposition: [
            'top right', 'top left', 'top center', 'bottom right', 'top right',
            'top left', 'bottom right', 'bottom left', 'top right', 'top right'
        ],
    }];
    
    var layout = {
        autosize: false, 
        width: 1000, 
        height: 1000,
        title: 'Canadian cities',
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
            landcolor: '#EAEAAE',
            countrycolor: '#d3d3d3',
            countrywidth: 1.5,
            subunitcolor: '#d3d3d3'
        }
    };
    
    Plotly.newPlot('myDiv', data, layout);

}
