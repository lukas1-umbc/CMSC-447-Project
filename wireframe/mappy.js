/***********************************************************************
mappy.js file updated by Kavya Kavanakudy on April 19, 2020.
This is the .js file that interacts with the API in order to display a 
map of a GEOJSON file provided by the algorithm
***********************************************************************/

//The source files for geojson data

var MDdistrictData ="districts.geojson"
var MDprecinctData = "precinct2010.geojson"
var SCprecinctData = "2013_sc_precincts.geojson"

/****************************************
  A varialbe that stores how the 
  geojson data will be displayed based
  on party.
****************************************/

var partyStyles = {
  
  //red fill and out line for Republican districts
  'Republican': new ol.style.Style({
    fill: new ol.style.Fill({
        color: 'rgba(193, 73, 73, 0.5)'
    }),
    stroke: new ol.style.Stroke({
        color: 'rgba(204, 0, 0, 1)',
        width: 3
    })
  }),
  
  //blue fill and oultine for Democratic districts
  'Democrat': new ol.style.Style({
    fill: new ol.style.Fill({
        color: 'rgba(73, 125, 193, 0.5)'
    }),
    stroke: new ol.style.Stroke({
        color: 'rgba(0, 0, 255, 1)',
        width: 3
    }),
  })

};

/************************************************* 
  A style function for the districts
  it choose a style based on party the feature is.
  It takes in a feature and returns the approciate
  style characteristics.
***************************************************/

var styleFunction = function(feature) {
  return partyStyles[feature.get("party")]
};

/******************************************************
Creates a group of one layer: one layer is the base map 
layer. This will be what the screen defaults too.
******************************************************/
var defaultGroup = new ol.layer.Group({
    layers:[
      new ol.layer.Tile({
        source: new ol.source.OSM()
      }),
      ]
})
/******************************************************
Creates a group of layers: one layer is the base map 
layer, the other is the MD district geojson data with
the appropriate style and the final layer is the 
MD precinct geojson Data
******************************************************/

var MDdistrictGroup = new ol.layer.Group({
    layers:[

      new ol.layer.Tile({
        source: new ol.source.OSM()
      }),

      new ol.layer.Vector({
        source: new ol.source.Vector({
        	url: MDdistrictData,
        	format: new ol.format.GeoJSON(),
        }),
        style: styleFunction
      }),
      new ol.layer.Vector({
      	source: new ol.source.Vector({
      		url: MDprecinctData,
      		format: new ol.format.GeoJSON(),
      	}),
      	style: new ol.style.Style({
        	stroke: new ol.style.Stroke({
          	color: 'rgba(0,0,0,1)',
          	width: .5
        	})
      	})
    })
    ]
})


/******************************************************
Creates a group of layers: one layer is the base map 
layer, and the other is the SC precinct geojson data with
the appropriate style. 
******************************************************/

var SCdistrictGroup = new ol.layer.Group({
  layers:[

    new ol.layer.Tile({
      source: new ol.source.OSM()
    }),

    new ol.layer.Vector({
      source: new ol.source.Vector({
      	url: SCprecinctData,
      	format: new ol.format.GeoJSON(),
      }),
      style: new ol.style.Style({
        stroke: new ol.style.Stroke({
          color: 'rgba(0,0,0,1)',
          width: .5
        })
      })
    })
  ]
})


/******************************************************
The following displays the map with an 
open street maps layer, and is centered on the USA
******************************************************/

var map = new ol.Map({

    target: 'map',

    layers: defaultGroup,

    view: new ol.View({
        //lon, lat coordinates for center of the USA
        center: ol.proj.fromLonLat([-98.58333333, 39.83333333]),
        zoom: 4
   })

});

/******************************************************
The following displays the apporitate state information
based on what the user selects after the Run button
is pushed
******************************************************/

var runButton = document.getElementById("mapswitcher");


runButton.addEventListener("click", function(){
  console.log("button clicked")
	
	var state = document.getElementById("selectState").value;

	if(state == 'Maryland'){
    	map.setLayerGroup(MDdistrictGroup)
    	map.setView(
    		new ol.View({
        		//lon, lat coordinates for Maryland
        		center: ol.proj.fromLonLat([-76.641273, 39.045753]),
        		zoom: 7.5
   			})
    	)
    }

    else if(state == 'South Carolina'){
    	map.setLayerGroup(SCdistrictGroup)
    	map.setView(
    		new ol.View({
        		//lon, lat coordinates for South Carolina
        		center: ol.proj.fromLonLat([-81.163727, 33.836082]),
        		zoom: 7.5
   			})
    	)
    }

   else{
   		window.alert("Please select a state")
   }
});

/******************************************************
The following resets the map to the deafults view
******************************************************/
var resetButton = document.getElementById("btnReset");

resetButton.addEventListener("click", function(){
    console.log("button clicked")
    map.setLayerGroup(defaultGroup)
    map.setView(
        new ol.View({
            //lon, lat coordinates for center of the USA
            center: ol.proj.fromLonLat([-98.58333333, 39.83333333]),
            zoom: 4
        })
    )
});
/******************************************************
The following displays the text user inputs
******************************************************/
var commentButton = document.getElementById("btnComment");

commentButton.addEventListener("click", function(){
    console.log("button clicked")
    console.log(CKEDITOR.instances.text.getData())
    document.getElementById("lblComment").innerHTML = CKEDITOR.instances.text.getData()
});

