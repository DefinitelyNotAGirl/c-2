console.log("hello world");

var output = document.getElementById("output");

var inputTable = document.getElementById("inputTable");

var rows = inputTable.children;

var javascriptIsShit = [];

function addJavascriptIsShit(mnemonic)
{
	for(let i = 0;i<javascriptIsShit.length;i++)
	{
		let item = javascriptIsShit[i];
		if(item[0] == mnemonic)
		{
			return;
		}
	}
	javascriptIsShit.push([mnemonic]);
}

for(let i = 1;i<rows.length;i++)
{
	var columns = rows.item(i).children.item(0).children;

	console.log("columns:",columns);
	let mnemonic = columns.item(10).innerHTML.toLowerCase();
	let opcode = columns.item(2).innerHTML;
	addJavascriptIsShit(mnemonic);
	var variant = "";
	if(columns.item(11).innerHTML)
		variant += columns.item(11).innerHTML;
	if(columns.item(12).innerHTML)
		variant += "§"+columns.item(12).innerHTML;
	if(columns.item(13).innerHTML)
		variant += "§"+columns.item(13).innerHTML;
	if(columns.item(14).innerHTML)
		variant += "§"+columns.item(14).innerHTML;
	if(variant == "")
		variant = "_";
	for(let i = 0;i<javascriptIsShit.length;i++)
	{
		let item = javascriptIsShit[i];
		if(item[0] == mnemonic)
		{
			item.push(variant);
			item.push(opcode);
		}
	}
}

console.log("opcodes:",javascriptIsShit);

for(let i = 0;i<javascriptIsShit.length;i++)
{
	let item = javascriptIsShit[i];
	var mnemonic = item[0];
	output.innerHTML += "namespace "+mnemonic+"{<br>";
	for(let v = 1;v<item.length;v+=2)
	{
		var operrands = item[v+0];
		var opcode = item[v+1];
		output.innerHTML += "constexpr byte "+operrands+" = 0x"+opcode+";<br>";
	}
	output.innerHTML += "}<br>";
}
