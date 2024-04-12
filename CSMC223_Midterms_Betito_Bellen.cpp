#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct instruct {
    string opcode;
    string data;
    string reg;
};

class Memory{
private:
    vector<string> data;

public:
    Memory(int size) : data(size) {}
    string read(int address) const {
        if (address >= 0 && address < data.size()) {
            return data[address];
        } 
        return "";
    }

    void write(int address, const string& value) {
        if (address >= 0 && address < data.size()) {
            data[address] = value;
        }
    }

    int dataSize() const{
        return data.size();
    }
};

class CPU {
private:
    unordered_map<string, int> registers;
    Memory memory;
    int pc; //stands for program counter

public:
    CPU(int memorySize) : memory(memorySize), pc(0){
        registers["GR[0]"] = 0;
        registers["GR[1]"] = 0;
        registers["GR[2]"] = 0;
        registers["GR[3]"] = 0;
        }

    void storeInst(const instruct& instr) {
        int index = stoi(instr.data);
        string reg = instr.reg;
        memory.write(index, to_string(registers[reg]));
        pc++;
    }

    void executeInstruction(const instruct& instr) {
        if (instr.opcode == "LOAD") {
            load(instr.data, instr.reg);
        } else if (instr.opcode == "STORE") {
            store(instr.data, instr.reg);
        } else if (instr.opcode == "PRINT") {
            print(instr.reg);
        } else if (instr.opcode == "ADD") {
            add(instr.data, instr.reg);
        } else if (instr.opcode == "SUBT") {
            subt(instr.data, instr.reg);
        } else if (instr.opcode == "MULT") {
            mult(instr.data, instr.reg);
        } else if (instr.opcode == "DIV") {
            div(instr.data, instr.reg);
        } else {
            cout << "Unknown opcode: " << instr.opcode << endl;
        }
        pc++;
    }

    void load(const string& data, const string& reg) {
        if (!data.empty()){
            registers[reg] = stoi(data);
        } else {
            cout << "No data provided for LOAD instructions." << endl;
        }
    }

    void store(const string& data, const string& reg) {
        if (!data.empty()){
            int index = stoi(data);
            if (index < 0 || index >= memory.dataSize()) {
                cout << "Invalid memory index" << endl;
                return;
            }
            memory.write(index, to_string(registers[reg]));
        } else {
            cout << "No data provided for STORE instruction." << endl;
        }
    }

    void print(const string& reg) {
        cout << "Data in register " << reg << ": " << registers[reg] << endl;
    }

    void add(const string& data, const string& reg){
        if (!data.empty()) {
            registers[reg] += stoi(data);
        } else {
            cout << "No data provided for ADD instruction." << endl;
        }
    }

    void subt(const string& data, const string& reg){
        if (!data.empty()) {
            registers[reg] -= stoi(data);
        } else {
            cout << "No data provided for SUBT instruction." << endl;
        }
    }

    void mult(const string& data, const string& reg){
        if (!data.empty()) {
            registers[reg] *= stoi(data);
        } else {
            cout << "No data provided for MULT instruction." << endl;
        }
    }

    void div(const string& data, const string& reg){
        if (!data.empty()) {
            registers[reg] /= stoi(data);
        } else {
            cout << "No data provided for DIV instruction." << endl;
        }
    }
};



int main() {
    int memorySize = 64 * 1024;
    CPU cpu(memorySize);

    ifstream inputFile("input.txt");
    if (!inputFile.is_open()){
        cout << "Failed to open the file." << endl;
        return 1;
    }

    string line;

    while(getline(inputFile, line)) {
        istringstream iss(line);
        string opcode, data, reg;

        if (iss >> opcode >> data >> reg) {
            instruct instr;
            instr.opcode = opcode;
            instr.data = data;
            instr.reg = reg;

            cpu.storeInst(instr);
            cpu.executeInstruction(instr);
        } else if (!line.empty()){
            cout << "Error parsing line : " << line << endl;
        }
    }
    inputFile.close();

    return 0;
}
