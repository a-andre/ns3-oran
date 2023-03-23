/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * NIST-developed software is provided by NIST as a public service. You may
 * use, copy and distribute copies of the software in any medium, provided that
 * you keep intact this entire notice. You may improve, modify and create
 * derivative works of the software or any portion of the software, and you may
 * copy and distribute such modifications or works. Modified works should carry
 * a notice stating that you changed the software and should note the date and
 * nature of any such change. Please explicitly acknowledge the National
 * Institute of Standards and Technology as the source of the software.
 *
 * NIST-developed software is expressly provided "AS IS." NIST MAKES NO
 * WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT OR ARISING BY OPERATION OF
 * LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT AND DATA ACCURACY. NIST
 * NEITHER REPRESENTS NOR WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE
 * UNINTERRUPTED OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST
 * DOES NOT WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE USE OF THE
 * SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE
 * CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.
 *
 * You are solely responsible for determining the appropriateness of using and
 * distributing the software and you assume all risks associated with its use,
 * including but not limited to the risks and costs of program errors,
 * compliance with applicable laws, damage to or loss of data, programs or
 * equipment, and the unavailability or interruption of operation. This
 * software is not intended to be used in any situation where a failure could
 * cause risk of injury or damage to property. The software developed by NIST
 * employees is not subject to copyright protection within the United States.
 */

#ifndef ORAN_LM_LTE_2_LTE_TORCH_HANDOVER
#define ORAN_LM_LTE_2_LTE_TORCH_HANDOVER

#include "oran-data-repository.h"
#include "oran-lm.h"

#include <ns3/vector.h>

#include <torch/script.h>
#include <vector>

namespace ns3
{

/**
 * \ingroup oran
 *
 * Logic Module for the Near-RT RIC that issues Commands to handover from
 * an LTE cell to another based on a PyTorch ML model.
 */
class OranLmLte2LteTorchHandover : public OranLm
{
  protected:
    /**
     * UE related information.
     */
    struct UeInfo
    {
        uint64_t nodeId; //!< The node ID.
        uint16_t cellId; //!< The cell ID.
        uint16_t rnti;   //!< The RNTI ID.
        double loss;     //!< The loss.
    };

    /**
     * eNB related information.
     */
    struct EnbInfo
    {
        uint64_t nodeId; //!< The node ID.
        uint16_t cellId; //!< The cell ID.
        Vector position; //!< The physical position.
    };

  public:
    /**
     * Gets the TypeId of the OranLmLte2LteTorchHandover class.
     *
     * \return The TypeId.
     */
    static TypeId GetTypeId(void);
    /**
     * Constructor of the OranLmLte2LteTorchHandover class.
     */
    OranLmLte2LteTorchHandover(void);
    /**
     * Destructor of the OranLmLte2LteTorchHandover class.
     */
    ~OranLmLte2LteTorchHandover(void) override;
    /**
     * Runs the logic specific for this Logic Module. This will retrieve the
     * location of all LTE UEs and eNBs and the application loss for all UEs,
     * calculate the distance between all eNBs for each UE, pass these as
     * inputs to the PyTorch ML model, and then generate zero or more handover
     * Commands based on the ML model output that is generated.
     *
     * \return A vector with the handover commands generated by this Logic Module.
     */
    std::vector<Ptr<OranCommand>> Run(void) override;
    /**
     * Sets the path of the trainined PyTorch ML model.
     *
     * \parm trochModelPath the file path of the PyTorch ML model.
     */
    void SetTorchModelPath(const std::string& torchModelPath);

  private:
    /**
     * The PyTorch ML model.
     */
    torch::jit::script::Module m_model;

    /**
     * Method to get the UE information from the repository.
     *
     * \param data The data repository.
     *
     * \return A vector of UE Information structures.
     */
    std::vector<OranLmLte2LteTorchHandover::UeInfo> GetUeInfos(Ptr<OranDataRepository> data) const;
    /**
     * Method to get the eNB information from the repository.
     *
     * \param data The data repository.
     *
     * \return A vector of eNB Information structures.
     */
    std::vector<OranLmLte2LteTorchHandover::EnbInfo> GetEnbInfos(
        Ptr<OranDataRepository> data) const;
    /**
     * Method with the logic to generate Handover Commands if needed.
     *
     * \param data The data repository.
     * \param ueInfos A vector with the UE information.
     * \param enbInfos A vector with the eNB information.
     *
     * \return A vector with the handover commands generated.
     */
    std::vector<Ptr<OranCommand>> GetHandoverCommands(
        Ptr<OranDataRepository> data,
        std::vector<OranLmLte2LteTorchHandover::UeInfo> ueInfos,
        std::vector<OranLmLte2LteTorchHandover::EnbInfo> enbInfos);

}; // class OranLmLte2LteTorchHandover

} // namespace ns3

#endif // ORAN_LM_LTE_2_LTE_TORCH_HANDOVER
