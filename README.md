Usually Multilayer Printed Circuit boards are designed by commercial/free PCB design tools. The insulation layer is provided by FR-4 substrates. The complete fabrication process
involves subtractive design, which requires cutting Copper or FR-4 layer to give shape to the desired design. But, for additive inkjet printing, there is no such ECAD 
design tools available. Usually circuits are designed using graphics editor (for example: Inkscape), once the circuit is designed for single layer, an insulation layer 
optimized (in terms of ink usage) is nessecary to print another layer on top. For mulitlayer design, insulation layer generation is laborsome and tedious. This project
helps the designer to make multilayer circuit design printable by Inkjet printer. 
## Input: Gerber files of signal layers designed by traditional PCB design tools. It makes signal layer generation fast, easy and reliable
## Output: 1 bit bitmap images (print format for inkjet printers) of input signal layers and the insulation layer automatically generated based on the input signal
layer design. 
