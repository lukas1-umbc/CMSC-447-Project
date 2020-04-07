window.onload = init;

function init()
{
	const map = new ol.Map({
		view: new ol.View({
			center: [-8537174.969248261, 4700000], //This is where the map will center, will try and set up for UMBC for now
			zoom: 8, //How zoomed in are we, can control how much user can zoom in/out later on
		}),
		
		 //layer of tile, ie one layer is map, another can outline precincts, another zip, etc
		layers:[
			new ol.layer.Tile({
				source: new ol.source.OSM() //the tile will get its data from Open Street Maps(OSM)
			})
		],
		
		//target = where is this javascript being placed in webpage (tied to id)
		target: 'openStreetJS'
		
		
	});
	
	map.on('click', function(e)
	{
		console.log(e.coordinate);
	});
}