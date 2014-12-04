#include<iostream>
#include<vector>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
#include<set>
#include<vector>
#include<cstdlib>
#include<cassert>
#include<ctime>
#include <cassert>
#include<cmath>
#include "stringutil.h"

#include"transplan.h"
#include"configure.h"

class FactoryReader
{
public:

    /// Factory Details
    int num_cutting_machines;
    int num_painting_machines;
    int num_assembly_areas;
    int slots_per_painting_machines;
    int num_drying_units;
    int num_colors;
    int num_parts;

    int num_locations;

    int try_count;

    typedef std::vector< std::vector<int> > intmatrix;
    typedef std::vector<int> intvector;

    intmatrix cuttingmachine_part_can_produce_matrix;
    intmatrix cuttingmachine_part_time_matrix;
    intmatrix cuttingmachine_part_config_matrix;
    intmatrix cuttingmachine_part_offload_matrix;
    intmatrix paint_color_part_matrix;
    intmatrix distance_matrix;
    intmatrix color_setup_matrix;

    intvector dry_time_parts;
    intvector cuttingmachines_clean_intervals;
    intvector worker_demand_parts;
    intvector drying_unit_cooling_period;
    intvector drying_unit_max_on_time;

    intvector drying_unit_setup_matrices;
    int distance_sq_matrix_id;
    int color_sq_matrix_id;


    intvector cutting_location_ids;
    intvector painting_location_ids;
    intvector drying_unit_location_ids;
    intvector asb_location_ids;

    /////////////////// Instance Data Structure ///////////////////////////
    int num_instances;
    int max_order;
    int min_order;
    int num_orders;
    int max_part_per_order;
    int min_part_per_order;
    int max_deadline_order;
    int min_deadline_order;
    int max_worker;
    int min_worker;
    int num_workers;
    int assembly_time_order_factor; //// factor to calculate assembly time depending on how many parts that order would need. #number_of_parts * factor = assembly time
    float assembly_worker_factor; //// fraction of worker need per part of the order

    intmatrix order_part_requirement;
    intmatrix order_part_color;
    intvector order_deadlines;
    intvector order_assembly_time;
    intvector order_assembly_worker;

    intmatrix dru_on_off_seq;

    ////////////////////////// Transplan Solver ///////////////////////////////////////////////////////////

    TRANSPLAN::Transplan planner;

    FactoryReader()
    {
        std::cout << "Reading a Factory Domain problem" << std::endl;
    }

    void Statistics()
    {
        std::cout << "Numner of Cutting Machines  : " << num_cutting_machines << std::endl;
        std::cout << "Numner of Painting Machines : " << num_painting_machines << std::endl;
        std::cout << "Numner of Assembly Areas    : " << num_assembly_areas << std::endl;
        std::cout << "Number of Drying Units      : " << num_drying_units << std::endl;
        std::cout << "Numner of Solts Per Machine : " << slots_per_painting_machines << std::endl;
        std::cout << "Numner of Colors            : " << num_colors << std::endl;
        std::cout << "Numner of Parts             : " << num_parts << std::endl;

    }

    void ReadCuttingMachineCapabilityMatrix(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line); /// ignore first
        std::getline(reader, line); ///ignore second

        for (int i = 0; i < num_cutting_machines; ++i)
        {
            cuttingmachine_part_can_produce_matrix.push_back(std::vector<int>());
            std::getline(reader, line); /// read a cutting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == num_parts + 1);
            for (int j = 0; j < num_parts; ++j)
            {
                cuttingmachine_part_can_produce_matrix[i].push_back(atoi(tokens[j + 1].c_str()));
            }
        }


    }

    void ReadCuttingMachineProductionTimeMatrix(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line); /// ignore first
        std::getline(reader, line); ///ignore second

        for (int i = 0; i < num_cutting_machines; ++i)
        {
            cuttingmachine_part_time_matrix.push_back(std::vector<int>());
            std::getline(reader, line); /// read a cutting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == num_parts + 1);
            for (int j = 0; j < num_parts; ++j)
            {
                cuttingmachine_part_time_matrix[i].push_back(atoi(tokens[j + 1].c_str()));
            }
        }

    }

    void ReadCuttingMachineConfigTimeMatrix(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line); /// ignore first
        std::getline(reader, line); ///ignore second

        for (int i = 0; i < num_cutting_machines; ++i)
        {
            cuttingmachine_part_config_matrix.push_back(std::vector<int>());
            std::getline(reader, line); /// read a cutting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == num_parts + 1);

            for (int j = 0; j < num_parts; ++j)
            {
                cuttingmachine_part_config_matrix[i].push_back(atoi(tokens[j + 1].c_str()));
            }
        }


    }

    void ReadCuttingMachineOffloadTimeMatrix(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);
        std::getline(reader, line);

        for (int i = 0; i < num_cutting_machines; ++i)
        {
            cuttingmachine_part_offload_matrix.push_back(std::vector<int>());
            std::getline(reader, line); /// read a cutting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == num_parts + 1);

            for (int j = 0; j < num_parts; ++j)
            {
                cuttingmachine_part_offload_matrix[i].push_back(atoi(tokens[j + 1].c_str()));
            }
        }

    }

    void ReadCuttingMachineCleaningIntervals(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);

        for (int i = 0; i < num_cutting_machines; i++)
        {
            std::getline(reader, line); /// read a cutting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == 2);
            cuttingmachines_clean_intervals.push_back(atoi(tokens[1].c_str()));
        }


    }

    void ReadPaintingMachineColoringTimeMatrix(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);
        std::getline(reader, line);

        for (int i = 0; i < num_painting_machines; ++i)
        {
            paint_color_part_matrix.push_back(std::vector<int>());
            std::getline(reader, line); /// read a painting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == num_parts + 1);

            for (int j = 0; j < num_parts; ++j)
            {
                paint_color_part_matrix[i].push_back(atoi(tokens[j + 1].c_str()));
            }
        }
    }

    void InitializeDistance(std::ifstream& reader, std::vector<int>& point)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line); /// read a painting machine
        tokens.clear();
        tokenize(line, tokens, std::string(" \t"));
        assert(tokens.size() == num_cutting_machines + num_painting_machines + num_drying_units + num_assembly_areas + 1);
        int locations = 0;
        for (int j = 0; j < num_cutting_machines; j++)
        {
            point.push_back(atoi(tokens[locations + 1].c_str()));
            locations++;
            cutting_location_ids.push_back(locations);

        }

        for (int j = 0; j < num_painting_machines; j++)
        {
            point.push_back(atoi(tokens[locations + 1].c_str()));
            locations++;
            painting_location_ids.push_back(locations);
        }

        for (int j = 0; j < num_drying_units; j++)
        {
            point.push_back(atoi(tokens[locations + 1].c_str()));
            locations++;
            drying_unit_location_ids.push_back(locations);
        }

        for (int j = 0; j < num_assembly_areas; j++)
        {
            point.push_back(atoi(tokens[locations + 1].c_str()));
            locations++;
            asb_location_ids.push_back(locations);
        }

        num_locations = locations;
    }

    void ReadDistanceMatrix(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);
        std::getline(reader, line);

        int location_count = 0;
        for (int i = 0; i < num_cutting_machines; i++)
        {
            distance_matrix.push_back(std::vector<int>());
            InitializeDistance(reader, distance_matrix[location_count]);
            location_count++;
        }
        for (int i = 0; i < num_painting_machines; i++)
        {
            distance_matrix.push_back(std::vector<int>());
            InitializeDistance(reader, distance_matrix[location_count]);
            location_count++;

        }

        for (int i = 0; i < num_drying_units; i++)
        {
            distance_matrix.push_back(std::vector<int>());
            InitializeDistance(reader, distance_matrix[location_count]);
            location_count++;
        }

        for (int i = 0; i < num_assembly_areas; i++)
        {
            distance_matrix.push_back(std::vector<int>());
            InitializeDistance(reader, distance_matrix[location_count]);
            location_count++;
        }

    }

    void ReadColorSetupTimeMatrix(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);
        std::getline(reader, line);

        for (int i = 0; i < num_colors; i++)
        {
            color_setup_matrix.push_back(std::vector<int>());
            std::getline(reader, line); /// read a painting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == num_colors + 1);

            for (int j = 0; j < num_colors; ++j)
            {
                color_setup_matrix[i].push_back(atoi(tokens[j + 1].c_str()));
            }
        }

    }

    void ReadDryingUnits(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);

        for (int i = 0; i < num_drying_units; i++)
        {
            std::getline(reader, line);
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == 3);

            drying_unit_cooling_period.push_back(atoi(tokens[1].c_str()));
            drying_unit_max_on_time.push_back(atoi(tokens[2].c_str()));
        }


    }

    void ReadPartsDryingTimes(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);

        for (int i = 0; i < num_parts; i++)
        {
            std::getline(reader, line); /// read a painting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == 2);

            dry_time_parts.push_back(atoi(tokens[1].c_str()));
        }
    }

    void ReadPartsWorkerDemands(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);

        for (int i = 0; i < num_parts; i++)
        {
            std::getline(reader, line); /// read a painting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == 2);

            worker_demand_parts.push_back(atoi(tokens[1].c_str()));
        }
    }

    void ReadDomain(std::string domain_file_name)
    {
        std::ifstream reader(domain_file_name.data());
        std::string line;
        std::vector<std::string> tokens;

        //// Number of CM
        std::getline(reader, line);
        tokens.clear();
        tokenize(line, tokens, std::string(" \t"));
        std::cout << " Reading domain file " << domain_file_name << " SIZE " << line << std::endl;
        for (int i = 0; i < tokens.size(); i++)
        {
            std::cout << tokens[1] << std::endl;
        }
        num_cutting_machines = atoi(tokens[1].c_str());
        //std::cout<<"HERE"<<"\n";
        //// Number of PM
        std::getline(reader, line);
        tokens.clear();
        tokenize(line, tokens, std::string(" \t"));
        num_painting_machines = atoi(tokens[1].c_str());


        //// Number of PM-Slots
        std::getline(reader, line);
        tokens.clear();
        tokenize(line, tokens, std::string(" \t"));
        slots_per_painting_machines = atoi(tokens[1].c_str());


        //// Number of ASB
        std::getline(reader, line);
        tokens.clear();
        tokenize(line, tokens, std::string(" \t"));
        num_assembly_areas = atoi(tokens[1].c_str());

        //// Number of DRU
        std::getline(reader, line);
        tokens.clear();
        tokenize(line, tokens, std::string(" \t"));
        num_drying_units = atoi(tokens[1].c_str());

        //// Number of Colors
        std::getline(reader, line);
        tokens.clear();
        tokenize(line, tokens, std::string(" \t"));
        num_colors = atoi(tokens[1].c_str());

        //// Number of Parts
        std::getline(reader, line);
        tokens.clear();
        tokenize(line, tokens, std::string(" \t"));
        num_parts = atoi(tokens[1].c_str());


        Statistics();

        ReadCuttingMachineCapabilityMatrix(reader);
        ReadCuttingMachineProductionTimeMatrix(reader);
        ReadCuttingMachineConfigTimeMatrix(reader);
        ReadCuttingMachineOffloadTimeMatrix(reader);
        ReadCuttingMachineCleaningIntervals(reader);
        ReadPaintingMachineColoringTimeMatrix(reader);
        ReadDistanceMatrix(reader);
        ReadColorSetupTimeMatrix(reader);
        ReadDryingUnits(reader);
        ReadPartsDryingTimes(reader);
        ReadPartsWorkerDemands(reader);

    }

    void ReadOrderPartRequirementMatrix(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);
        std::getline(reader, line);

        for (int i = 0; i < num_orders; i++)
        {
            order_part_requirement.push_back(std::vector<int>());
            std::getline(reader, line); /// read a painting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == num_parts + 1);

            for (int j = 0; j < num_parts; j++)
            {
                order_part_requirement[i].push_back(atoi(tokens[j + 1].c_str()));
            }
        }

    }

    void ReadOrderPartColorRequirements(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);

        for (int i = 0; i < num_orders; i++)
        {
            order_part_color.push_back(std::vector<int>());
            std::getline(reader, line); /// read a painting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            int number_of_colors = atoi(tokens[1].c_str());
            for (int j = 0; j < number_of_colors; j++)
            {
                //std::cout << atoi( tokens[ j + 2].c_str() ) << std::endl;
                order_part_color[i].push_back(atoi(tokens[ j + 2].c_str()));

            }
        }
    }

    void ReadOrderDeadlines(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);

        for (int i = 0; i < num_orders; i++)
        {
            std::getline(reader, line); /// read a painting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == 2);

            order_deadlines.push_back(atoi(tokens[1].c_str()));
        }


    }

    void ReadOrderAssemblyTimes(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);

        for (int i = 0; i < num_orders; i++)
        {
            std::getline(reader, line); /// read a painting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == 2);

            order_assembly_time.push_back(atoi(tokens[1].c_str()));
        }
    }

    void ReadOrderWorkerDemands(std::ifstream& reader)
    {
        std::string line;
        std::vector<std::string> tokens;
        std::getline(reader, line);

        for (int i = 0; i < num_orders; i++)
        {
            std::getline(reader, line); /// read a painting machine
            tokens.clear();
            tokenize(line, tokens, std::string(" \t"));
            assert(tokens.size() == 2);

            order_assembly_worker.push_back(atoi(tokens[1].c_str()));
        }


    }

    void ReadInstance(std::string instance_file_name)
    {
        /// read number of orders and number of workers
        std::ifstream reader(instance_file_name.data());
        std::string line;
        std::vector<std::string> tokens;

        std::getline(reader, line); /// read a painting machine
        //std::cout << line << std::endl;

        tokens.clear();
        tokenize(line, tokens, std::string(" \t="));
        //std::cout << tokens[0] << " " << tokens[1] << std::endl;
        num_orders = atoi(tokens[1].c_str());

        std::getline(reader, line); /// read a painting machine
        //std::cout<< line << std::endl;
        tokens.clear();
        tokenize(line, tokens, std::string(" \t="));
        //std::cout << tokens[0] << " " << tokens[1] << std::endl;
        num_workers = atoi(tokens[1].c_str());

        std::cout << "Reading problem instance.........\n";
        std::cout << "Number of orders in the problem : " << num_orders << "\n"
                << "Number of workers in the problem : " << num_workers << std::endl;

        ReadOrderPartRequirementMatrix(reader);
        ReadOrderPartColorRequirements(reader);
        ReadOrderDeadlines(reader);
        ReadOrderAssemblyTimes(reader);
        ReadOrderWorkerDemands(reader);


    }
    /////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////// Create Model  /////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

    void CreateMatrices()
    {
        /// create distance matrix
        //std::cout << "Num of locations " << num_locations + 1 << std::endl;;
        int id = distance_sq_matrix_id = planner.createNewSquareMatrix(num_locations + 1, 0);
        for (int i = 0; i < num_locations; i++)
        {
            for (int j = 0; j < num_locations; j++)
            {
                planner.sqmatrices[ distance_sq_matrix_id].setElementValue(i + 1, j + 1, distance_matrix[i][j]);
            }
        }

        //std::cout << "Distance Matrix "<< id << std::endl;
        //planner.sqmatrices[ id ].print();

        /// create color setup  matrix
        id = color_sq_matrix_id = planner.createNewSquareMatrix(num_colors + 1, 0);
        for (int i = 0; i < num_colors; i++)
        {
            for (int j = 0; j < num_colors; j++)
            {
                planner.sqmatrices[color_sq_matrix_id].setElementValue(i + 1, j + 1, color_setup_matrix[i][j]);
            }
        }
        //std::cout << "Color Matrix " << id  << std::endl;
        //planner.sqmatrices[ id ].print();
        ////// create setup matrices for drying unit OFF-->ON = cooling period, 3 states: default(0), ON(1), OFF(2)

        for (int j = 0; j < num_drying_units; j++)
        {
            int mat_id = planner.createNewSquareMatrix(3, 0);
            planner.sqmatrices[mat_id].setElementValue(2, 1, drying_unit_cooling_period[j]);
            drying_unit_setup_matrices.push_back(mat_id);

            //std::cout << "DRU " << j << " Matrix " << mat_id << std::endl;
            //planner.sqmatrices[ mat_id ].print();
        }
    }

    void CreateStateVariabels()
    {
        ///we create state variables for each order
        for (int i = 0; i < num_orders; i++)
        {
            std::stringstream o_name;
            o_name << "Order-" << i;
            int id_o = planner.createNewStateVariable(o_name.str());

            //std::cout << planner.getSVIndex( o_name.str() ) << " " << planner.getSVIndex( "RAJA" ) << std::endl;

            std::string o_state_1 = "NOT_ASB";
            std::string o_state_2 = "ASB";
            planner.statevars[id_o].add_domain_value(o_state_1);
            planner.statevars[id_o].add_domain_value(o_state_2);

            //planner.statevars[id_o].print();
            //intial state
            planner.statevars[id_o].set_init_state(o_state_1);
            /// goal state
            planner.statevars[id_o].set_goal_state(o_state_2);

            ///we create state variables for each order-part
            for (int j = 0; j < num_parts; j++)
            {
                for (int k = 0; k < order_part_requirement[i][j]; k++)
                {
                    std::stringstream op_name;
                    op_name << "Order_" << i << "-Part_" << j << "-" << k;
                    int id_op = planner.createNewStateVariable(op_name.str());
                    std::string op_state_1 = "NOT_MADE";
                    std::string op_state_2 = "MADE";
                    std::string op_state_3 = "COLORED";
                    std::string op_state_4 = "DRIED";


                    planner.statevars[id_op].add_domain_value(op_state_1);
                    planner.statevars[id_op].add_domain_value(op_state_2);
                    planner.statevars[id_op].add_domain_value(op_state_3);
                    planner.statevars[id_op].add_domain_value(op_state_4);


                    //planner.statevars[id_op].print();
                    //intial state
                    planner.statevars[id_op].set_init_state(op_state_1);
                    /// add non-goal state: Order part can't finish at MADE or COLORED or DRIED state, either NOT_MADE or ASB
                    //planner.statevars[id_op].add_non_goal_state( op_state_2 );
                    //planner.statevars[id_op].add_non_goal_state( op_state_3 );
                    planner.statevars[id_op].set_goal_state(op_state_4);
                    /// add setup matrix
                    planner.statevars[id_op].setup_matrix_id = distance_sq_matrix_id;


                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////
        // Create State Variable for each drying unit with two states: OFF(start, goal), ON

        for (int i = 0; i < num_drying_units; i++)
        {
            dru_on_off_seq.push_back(intvector());
            std::stringstream op_name;
            op_name << "DRU_STATUS-" << i;
            int id_dru = planner.createNewStateVariable(op_name.str());
            std::string dru_state_1 = "OFF";
            std::string dru_state_2 = "ON";

            planner.statevars[id_dru].add_domain_value(dru_state_1);
            planner.statevars[id_dru].add_domain_value(dru_state_2);

            //planner.statevars[id_dru].print();

            //intial state
            planner.statevars[id_dru].set_init_state(dru_state_1);
            /// goal state
            planner.statevars[id_dru].set_goal_state(dru_state_1);

            /// set max persistent value for the state ON
            planner.statevars[id_dru].setMaxPersist(dru_state_2, drying_unit_max_on_time[i]);

            /// add setup matrix
            planner.statevars[id_dru].setup_matrix_id = drying_unit_setup_matrices[i];
        }

    }

    void CreateResources()
    {
        // Cutting machines
        for (int i = 0; i < num_cutting_machines; i++)
        {
            std::stringstream r_name;
            r_name << "CM_" << i;
            int id_cm = planner.createNewResource(r_name.str(), 1, TRANSPLAN::REUSE_R);

            // Cutting machines-waste-collector
            std::stringstream r_c_name;
            r_c_name << "CM_" << i << "_Waste";
            int id_cmw = planner.createNewResource(r_c_name.str(), cuttingmachines_clean_intervals[i], TRANSPLAN::RESERV_R);
            planner.setResourceInitLevel(id_cmw, 0);
        }

        // Painting machines
        for (int i = 0; i < num_painting_machines; i++)
        {
            std::stringstream p_name;
            p_name << "PM_" << i;
            //int id_pm = planner.createNewResource( p_name.str(), slots_per_painting_machines, TRANSPLAN::REUSE_R);
            int id_pm = planner.createNewResource(p_name.str(), 1, TRANSPLAN::REUSE_R);

            /// add modes
            // add setup matrix
            planner.resources[id_pm].setup_matrix_id = color_sq_matrix_id;
        }


        // assembly area
        //for( int i = 0; i < num_assembly_areas; i++ )
        //{
        std::stringstream asb_name;
        asb_name << "ASB_AREA";
        int id_asb = planner.createNewResource(asb_name.str(), num_assembly_areas, TRANSPLAN::REUSE_R);
        planner.resources[id_asb].setup_matrix_id = distance_sq_matrix_id;
        //}

        // Worker pool
        std::stringstream worker_pool;
        worker_pool << "Workers";
        int id_worker_pool = planner.createNewResource(worker_pool.str(), num_workers, TRANSPLAN::REUSE_R);

        /// add setup matrix;
        planner.resources[id_worker_pool].setup_matrix_id = distance_sq_matrix_id;
    }

    void CreateActions()
    {
        int total_order_part = 0;
        /// create an action for each order to assemble
        for (int o = 0; o < num_orders; o++)
        {
            int op_count = 0;
            for (int p = 0; p < num_parts; p++)
            {
                for (int op = 0; op < order_part_requirement[o][p]; op++)
                {
                    total_order_part++;
                    std::stringstream op_name;
                    op_name << "Order_" << o << "-Part_" << p << "-" << op;
                    int sv_order_part_idx = planner.getSVIndex(op_name.str()); //sv_name_index_map[ op_name.str() ];


                    ///// create make order part action for each cutting machine
                    for (int cm = 0; cm < num_cutting_machines; cm++)
                    {
                        if (cuttingmachine_part_can_produce_matrix[cm][p])
                        {
                            CreateMakeOrderPartActon(cm, o, sv_order_part_idx, p, op);
                        }
                    }
                    //// create color order part action for each painting machine
                    for (int pm = 0; pm < num_painting_machines; pm++)
                    {
                        int color = order_part_color[o][op_count];
                        CreateColorOrderPartActon(pm, o, sv_order_part_idx, p, op, color);
                    }
                    /// create dry order part action
                    for (int dru = 0; dru < num_drying_units; dru++)
                    {
                        CreateDryOrderPartActon(dru, o, sv_order_part_idx, p, op);
                    }
                    op_count++;
                }
            }

            //// create ASB-Order action for each asb area
            for (int asb = 0; asb < num_assembly_areas; asb++)
            {
                CreateASBOrderAction(o, asb);
            }

        }


        /// create clean actions for cutting machines == total number = ceil(num_order/3)
        std::cout << "Total Order Part = " << total_order_part << std::endl;

        for (int cm = 0; cm < num_cutting_machines; cm++)
        {
            int interval = cuttingmachines_clean_intervals[ cm ];
            int total_clean_actions = total_order_part / interval;
            std::cout << "Clean actions = " << total_clean_actions << std::endl;
            for (int rp = 0; rp < total_clean_actions; rp++)
            {
                CreateCleanCMAction(cm, rp);
            }
        }

        //// create enough ON OFF action for each DRU
        int total_order_parts = 0;
        for (int o = 0; o < num_orders; o++)
        {
            for (int p = 0; p < num_parts; p++)
            {
                total_order_parts += order_part_requirement[o][p];
            }
        }
        //std::cout << "Total number of order-parts = " << total_order_parts << std::endl;
        int numer_of_On_OFF = 10; //ceil(total_order_parts/500); 

        for (int dru = 0; dru < num_drying_units; dru++)
        {
            for (int rp = 0; rp < numer_of_On_OFF; rp++)
            {
                CreateDruONOFFAction(dru, rp);
            }
        }

        for (int dru = 0; dru < num_drying_units; dru++)
        {
            ///add on-off sequences
            intvector seq = dru_on_off_seq[ dru ];
            for (int i = 0; i < seq.size() - 1; i++)
            {
                int from = seq[i];
                int to = seq[i + 1];
                //std::cout << "Adding from model \n";
                planner.addActPrecedence(from, to);
            }
        }




    }

    void CreateASBOrderAction(int order_id, int asb_id)
    {
        std::stringstream act_name;
        act_name << "AssembleOrder-" << order_id << "-" << asb_id;

        int or_asb_act_id = planner.createNewActivity(act_name.str());

        std::stringstream o_name;
        o_name << "Order-" << order_id;

        int sv_order_idx = planner.getSVIndex(o_name.str()); //sv_name_index_map[ o_name.str() ];

        //// add a transition that changes sv_order: NOT_ASB --> ASB
        std::string o_state_1 = "NOT_ASB";
        std::string o_state_2 = "ASB";
        int sv_from = planner.statevars[ sv_order_idx ].getStateIndex(o_state_1);
        int sv_to = planner.statevars[ sv_order_idx ].getStateIndex(o_state_2);
        int dur = order_assembly_time[ order_id ];

        int setup_state = asb_location_ids[ asb_id ];
        int mode = 0;
        int offset = 0;

        planner.createNewStateVariableTransition(sv_order_idx,
                or_asb_act_id,
                sv_from,
                sv_to,
                dur,
                offset,
                0);

        for (int p = 0; p < num_parts; p++)
        {
            for (int op = 0; op < order_part_requirement[order_id][p]; op++)
            {
                std::stringstream op_name;
                op_name << "Order_" << order_id << "-Part_" << p << "-" << op;
                int sv_order_part_idx = planner.getSVIndex(op_name.str()); //sv_name_index_map[ op_name.str() ];

                std::string op_state = "DRIED";

                int sv_op_state_idx = planner.statevars[ sv_order_part_idx ].getStateIndex(op_state);

                //// create a PREVAIL transition
                planner.createNewStateVariableTransition(sv_order_part_idx, ///state var idx
                        or_asb_act_id, ///act_idx
                        sv_op_state_idx, /// from state
                        sv_op_state_idx, /// to state
                        dur, ///duration
                        offset, ///offset from start
                        setup_state); ///setup state

            }
        }

        /// it borrows the abs location for the duration
        std::stringstream asb_name;
        asb_name << "ASB_AREA";
        int res_index = planner.getResIndex(asb_name.str()); //r_name_index_map[ asb_name.str() ];

        planner.createNewResourceTransition(res_index, /// resoource idx 
                or_asb_act_id, // act_idx
                1, /// requirement amount
                TRANSPLAN::BRR, //BORROW
                dur, /// duration
                offset, /// offset from start
                0, ////setup state /// no setup state
                mode); /// mode

        /// it takes some workers to assemble
        std::stringstream worker_pool;
        worker_pool << "Workers";
        int worker_res = planner.getResIndex(worker_pool.str()); //r_name_index_map[ worker_pool.str() ];
        planner.createNewResourceTransition(worker_res,
                or_asb_act_id,
                order_assembly_worker[ order_id ],
                TRANSPLAN::BRR,
                dur,
                offset,
                setup_state,
                mode);

    }

    void CreateMakeOrderPartActon(int cm_id, int order_id, int sv_order_part_idx, int part_id, int repeat)
    {
        std::stringstream act_name;
        act_name << "MakeOrderPart-" << order_id << "-" << part_id << "-" << repeat << "-CM-" << cm_id;

        int act_id = planner.createNewActivity(act_name.str());

        int config_time = cuttingmachine_part_config_matrix[cm_id][part_id];
        int offload_time = cuttingmachine_part_offload_matrix[cm_id][part_id];
        int production_time = cuttingmachine_part_time_matrix[cm_id][part_id];

        int total_time = config_time + production_time + offload_time;

        /// there will be 5 transitions
        //1. Config_Worker borrow transition    
        std::stringstream worker_pool;
        worker_pool << "Workers";
        int worker_res = planner.getResIndex(worker_pool.str()); //r_name_index_map[ worker_pool.str() ];
        int dur1 = config_time;
        int offset1 = 0;
        int mode1 = 0;
        int req1 = 1;
        int setup_state1 = cutting_location_ids[cm_id];

        planner.createNewResourceTransition(worker_res,
                act_id,
                req1,
                TRANSPLAN::BRR,
                dur1,
                offset1,
                setup_state1,
                mode1);

        //3. CM Resource borrow transition
        std::stringstream cm_name;
        cm_name << "CM_" << cm_id;
        int cm_res = planner.getResIndex(cm_name.str()); //r_name_index_map[ cm_name.str() ];
        int dur2 = total_time;
        int offset2 = 0;
        int mode2 = 0;
        int req2 = 1;
        int setup_state2 = 0;

        planner.createNewResourceTransition(cm_res,
                act_id,
                req2,
                TRANSPLAN::BRR,
                dur2,
                offset2,
                setup_state2,
                mode2);

        //4. Producing waste transition
        std::stringstream cm_w_name;
        cm_w_name << "CM_" << cm_id << "_Waste";
        int cm_w_res = planner.getResIndex(cm_w_name.str()); // r_name_index_map[ cm_w_name.str() ];
        int dur3 = production_time;
        int offset3 = config_time;
        int mode3 = 0;
        int req3 = 1;
        int setup_state3 = 0;

        planner.createNewResourceTransition(cm_w_res,
                act_id,
                req3,
                TRANSPLAN::PROD,
                dur3,
                offset3,
                setup_state3,
                mode3);

        //4. Off_load_Worker borrow transition
        int dur4 = offload_time;
        int offset4 = config_time + production_time;
        int mode4 = 0;
        int req4 = 1;
        int setup_state4 = setup_state1;

        planner.createNewResourceTransition(worker_res,
                act_id,
                req4,
                TRANSPLAN::BRR,
                dur4,
                offset4,
                setup_state4,
                mode4);

        //2. Overall State Change Make transition
        std::string op_state_1 = "NOT_MADE";
        std::string op_state_2 = "MADE";
        int from_state_idx = planner.statevars[ sv_order_part_idx ].getStateIndex(op_state_1);
        int to_state_idx = planner.statevars[ sv_order_part_idx ].getStateIndex(op_state_2);
        int dur5 = total_time;
        int offset5 = 0;
        int setup_state5 = setup_state1;

        planner.createNewStateVariableTransition(sv_order_part_idx,
                act_id,
                from_state_idx,
                to_state_idx,
                dur5,
                offset5,
                setup_state5);
    }

    void CreateColorOrderPartActon(int pm_id, int order_id, int sv_order_part_idx, int part_id, int repeat, int color)
    {
        std::stringstream act_name;
        act_name << "ColorOrderPart-" << order_id << "-" << part_id << "-" << repeat << "-PM-" << pm_id;
        int act_id = planner.createNewActivity(act_name.str());

        /// THERE are 2 transition
        //1. State variable transition
        std::string op_state_1 = "MADE";
        std::string op_state_2 = "COLORED";
        int from_state_idx = planner.statevars[ sv_order_part_idx ].getStateIndex(op_state_1);
        int to_state_idx = planner.statevars[ sv_order_part_idx ].getStateIndex(op_state_2);
        int dur1 = paint_color_part_matrix[pm_id][part_id];
        int offset1 = 0;
        int setup_state1 = painting_location_ids[pm_id];

        planner.createNewStateVariableTransition(sv_order_part_idx,
                act_id,
                from_state_idx,
                to_state_idx,
                dur1,
                offset1,
                setup_state1);
        //2. Resource transition
        std::stringstream pm_name;
        pm_name << "PM_" << pm_id;
        int pm_res = planner.getResIndex(pm_name.str()); //r_name_index_map[ pm_name.str() ];
        assert(pm_res > -1);
        int dur2 = dur1;
        int offset2 = 0;
        int mode2 = color + 1;
        int req2 = 1;
        int setup_state2 = color + 1;

        planner.createNewResourceTransition(pm_res,
                act_id,
                req2,
                TRANSPLAN::BRR,
                dur2,
                offset2,
                setup_state2,
                mode2);


    }

    void CreateDryOrderPartActon(int dru_id, int order_id, int sv_order_part_idx, int part_id, int repeat)
    {
        std::stringstream act_name;
        act_name << "DryOrderPart-" << order_id << "-" << part_id << "-" << repeat << "-DRU-" << dru_id;

        int act_id = planner.createNewActivity(act_name.str());

        std::string op_state_1 = "COLORED";
        std::string op_state_2 = "DRIED";

        int from_state_idx = planner.statevars[ sv_order_part_idx ].getStateIndex(op_state_1);
        int to_state_idx = planner.statevars[ sv_order_part_idx ].getStateIndex(op_state_2);
        int dur1 = dry_time_parts[ part_id ];
        int offset1 = 0;
        int setup_state1 = drying_unit_location_ids[ dru_id ];

        planner.createNewStateVariableTransition(sv_order_part_idx,
                act_id,
                from_state_idx,
                to_state_idx,
                dur1,
                offset1,
                setup_state1);


        ///2. Create a PREVAIL transition on DRYING UNIT STATUS
        std::stringstream dru_name;
        dru_name << "DRU_STATUS-" << dru_id;
        int sv_dru_idx = planner.getSVIndex(dru_name.str()); //sv_name_index_map[ dru_name.str() ];
        std::string dru_state_1 = "ON";

        int from_state_idx2 = planner.statevars[ sv_dru_idx ].getStateIndex(dru_state_1);
        int to_state_idx2 = from_state_idx2;

        int dur2 = dur1;
        int offset2 = 0;
        int setup_state2 = 0;

        planner.createNewStateVariableTransition(sv_dru_idx,
                act_id,
                from_state_idx2,
                to_state_idx2,
                dur2,
                offset2,
                setup_state2);



    }

    void CreateCleanCMAction(int cm_id, int repeat)
    {
        std::stringstream act_name;
        act_name << "CleanCM-" << cm_id << "-" << repeat;

        int act_id = planner.createNewActivity(act_name.str());

        /////2. A worker is needed
        std::stringstream worker_pool;
        worker_pool << "Workers";

        int worker_res = planner.getResIndex(worker_pool.str()); //r_name_index_map[ worker_pool.str() ];
        int dur1 = 10;
        int offset1 = 0;
        int mode1 = 0;
        int req1 = 1;
        int setup_state1 = cutting_location_ids[cm_id];

        planner.createNewResourceTransition(worker_res,
                act_id,
                req1,
                TRANSPLAN::BRR,
                dur1,
                offset1,
                setup_state1,
                mode1);

        //2. Transition to clean the waste
        std::stringstream cm_w_name;
        cm_w_name << "CM_" << cm_id << "_Waste";
        int cm_w_res = planner.getResIndex(cm_w_name.str()); //r_name_index_map[ cm_w_name.str() ];
        int dur2 = 10;
        int offset2 = 0;
        int mode2 = 0;
        int req2 = cuttingmachines_clean_intervals[ cm_id ]; /// clean all waste
        int setup_state2 = 0;

        planner.createNewResourceTransition(cm_w_res,
                act_id,
                req2,
                TRANSPLAN::CONS,
                dur2,
                offset2,
                setup_state2,
                mode2);



    }

    void CreateDruONOFFAction(int dru_id, int repeat)
    {
        std::stringstream on_act_name;
        on_act_name << "ON-DRU-" << dru_id << "-" << repeat;

        int on_act_id = planner.createNewActivity(on_act_name.str());

        std::stringstream dru_name;
        dru_name << "DRU_STATUS-" << dru_id;
        int sv_dru_idx = planner.getSVIndex(dru_name.str()); //sv_name_index_map[ dru_name.str() ];
        std::string dru_state_1 = "ON";
        std::string dru_state_2 = "OFF";

        int from_state_idx1 = planner.statevars[ sv_dru_idx ].getStateIndex(dru_state_2); ///OFF
        int to_state_idx1 = planner.statevars[ sv_dru_idx ].getStateIndex(dru_state_1); ///ON

        int dur1 = 1;
        int offset1 = 0;
        int setup_state1 = 1; // 1 == ON

        planner.createNewStateVariableTransition(sv_dru_idx,
                on_act_id,
                from_state_idx1,
                to_state_idx1,
                dur1,
                offset1,
                setup_state1);


        std::stringstream off_act_name;
        off_act_name << "OFF-DRU-" << dru_id << "-" << repeat;

        int off_act_id = planner.createNewActivity(off_act_name.str());

        int from_state_idx2 = planner.statevars[ sv_dru_idx ].getStateIndex(dru_state_1); ///ON
        int to_state_idx2 = planner.statevars[ sv_dru_idx ].getStateIndex(dru_state_2); ///OFF

        int dur2 = 1;
        int offset2 = 0;
        int setup_state2 = 2; // 2 == OFF

        planner.createNewStateVariableTransition(sv_dru_idx,
                off_act_id,
                from_state_idx2,
                to_state_idx2,
                dur2,
                offset2,
                setup_state2);

        planner.addActInplan(on_act_id);
        planner.addActInplan(off_act_id);

        //std::cout << on_act_id << "--> " << off_act_id << std::endl;
        dru_on_off_seq[dru_id].push_back(on_act_id);
        dru_on_off_seq[dru_id].push_back(off_act_id);

    }



};

int main(int argc, char* argv[])
{
    //std::cout << argc << std::endl; 
    if (argc == 3)
    {

        FactoryReader* fr = new FactoryReader();

        fr->ReadDomain(argv[1]);
        fr->ReadInstance(argv[2]);
        ////
        std::cout << "Creating planning model....................\n";
        std::cout << "Creating sq matrices...." << std::endl;
        fr->CreateMatrices();
        std::cout << "Creating state variables...." << std::endl;
        fr->CreateStateVariabels();

        std::cout << "Creating resources...." << std::endl;
        fr->CreateResources();

        std::cout << "Creating actions...." << std::endl;
        fr->CreateActions();

        //fr->planner.print_stat();
        fr->planner.problem_name = argv[2];
        fr->planner.initialize();
        fr->planner.search();
        /*
        for( int i = 0; i < fr->num_cutting_machines; i++ )
          {
            std::cout << "CM location " << fr->cutting_location_ids[i] << std::endl; 
          }
        for( int i = 0; i < fr->num_painting_machines; i++ )
          {
            std::cout << "PM location " << fr->painting_location_ids[i] << std::endl; 
          }
        for( int i = 0; i < fr->num_drying_units; i++ )
          {
            std::cout << "DRU location " << fr->drying_unit_location_ids[i] << std::endl; 
          }
        for( int i = 0; i < fr->num_assembly_areas; i++ )
          {
            std::cout << "ASB location " << fr->asb_location_ids[i] << std::endl; 
          }
         */
        delete fr;
    } else
    {
        std::cout << "Usage : ./factory_read <Domain> <Instance> " << std::endl;
    }

}
