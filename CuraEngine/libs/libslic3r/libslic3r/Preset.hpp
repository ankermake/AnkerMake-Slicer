#ifndef slic3r_Preset_hpp_
#define slic3r_Preset_hpp_

#include <deque>
#include <set>
#include <unordered_map>

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include "PrintConfig.hpp"
#include "Semver.hpp"

namespace Slic3r {

class AppConfig;
class PresetBundle;

enum ConfigFileType
{
    CONFIG_FILE_TYPE_UNKNOWN,
    CONFIG_FILE_TYPE_APP_CONFIG,
    CONFIG_FILE_TYPE_CONFIG,
    CONFIG_FILE_TYPE_CONFIG_BUNDLE,
};

extern ConfigFileType guess_config_file_type(const boost::property_tree::ptree &tree);

class VendorProfile
{
public:
    std::string                     name;
    std::string                     id;
    Semver                          config_version;
    std::string                     config_update_url;
    std::string                     changelog_url;

    struct PrinterVariant {
        PrinterVariant() {}
        PrinterVariant(const std::string &name) : name(name) {}
        std::string                 name;
    };

    struct PrinterModel {
        PrinterModel() {}
        std::string                 id;
        std::string                 name;
        PrinterTechnology           technology;
        std::string                 family;
        std::vector<PrinterVariant> variants;
        std::vector<std::string>	default_materials;
        // Vendor & Printer Model specific print bed model & texture.
        std::string 			 	bed_model;
        std::string 				bed_texture;

        PrinterVariant*       variant(const std::string &name) {
            for (auto &v : this->variants)
                if (v.name == name)
                    return &v;
            return nullptr;
        }

        const PrinterVariant* variant(const std::string &name) const { return const_cast<PrinterModel*>(this)->variant(name); }
    };
    std::vector<PrinterModel>          models;

    std::set<std::string>              default_filaments;
    std::set<std::string>              default_sla_materials;

    VendorProfile() {}
    VendorProfile(std::string id) : id(std::move(id)) {}

    bool 		valid() const { return ! name.empty() && ! id.empty() && config_version.valid(); }

    // Load VendorProfile from an ini file.
    // If `load_all` is false, only the header with basic info (name, version, URLs) is loaded.
    static VendorProfile from_ini(const boost::filesystem::path &path, bool load_all=true);
    static VendorProfile from_ini(const boost::property_tree::ptree &tree, const boost::filesystem::path &path, bool load_all=true);

    size_t      num_variants() const { size_t n = 0; for (auto &model : models) n += model.variants.size(); return n; }
    std::vector<std::string> families() const;

    bool        operator< (const VendorProfile &rhs) const { return this->id <  rhs.id; }
    bool        operator==(const VendorProfile &rhs) const { return this->id == rhs.id; }
};

class Preset;

// Helper to hold a profile with its vendor definition, where the vendor definition may have been extracted from a parent system preset.
// The parent preset is only accessible through PresetCollection, therefore to allow definition of the various is_compatible_with methods
// outside of the PresetCollection, this composite is returned by PresetCollection::get_preset_with_vendor_profile() when needed.
struct PresetWithVendorProfile {
	PresetWithVendorProfile(const Preset &preset, const VendorProfile *vendor) : preset(preset), vendor(vendor) {}
	const Preset 		&preset;
	const VendorProfile *vendor;
};

// Note: it is imporant that map is used here rather than unordered_map,
// because we need iterators to not be invalidated,
// because Preset and the ConfigWizard hold pointers to VendorProfiles.
// XXX: maybe set is enough (cf. changes in Wizard)
typedef std::map<std::string, VendorProfile> VendorMap;

class Preset
{
public:
    enum Type
    {
        TYPE_INVALID,
        TYPE_PRINT,
        TYPE_SLA_PRINT,
        TYPE_FILAMENT,
        TYPE_SLA_MATERIAL,
        TYPE_PRINTER,
        // This type is here to support PresetConfigSubstitutions for physical printers, however it does not belong to the Preset class,
        // PhysicalPrinter class is used instead.
        TYPE_PHYSICAL_PRINTER,
    };

    Preset(Type type, const std::string &name, bool is_default = false) : type(type), is_default(is_default), name(name) {}

    Type                type        = TYPE_INVALID;

    // The preset represents a "default" set of properties,
    // pulled from the default values of the PrintConfig (see PrintConfigDef for their definitions).
    bool                is_default;
    // External preset points to a configuration, which has been loaded but not imported
    // into the Slic3r default configuration location.
    bool                is_external = false;
    // System preset is read-only.
    bool                is_system   = false;
    // Preset is visible, if it is associated with a printer model / variant that is enabled in the AppConfig
    // or if it has no printer model / variant association.
    // Also the "default" preset is only visible, if it is the only preset in the list.
    bool                is_visible  = true;
    // Has this preset been modified?
    bool                is_dirty    = false;
    // Is this preset compatible with the currently active printer?
    bool                is_compatible = true;

    bool                is_user() const { return ! this->is_default && ! this->is_system; }

    // Name of the preset, usually derived form the file name.
    std::string         name;
    // File name of the preset. This could be a Print / Filament / Printer preset, 
    // or a Configuration file bundling the Print + Filament + Printer presets (in that case is_external and possibly is_system will be true),
    // or it could be a G-code (again, is_external will be true).
    std::string         file;
    // If this is a system profile, then there should be a vendor data available to display at the UI.
    const VendorProfile *vendor      = nullptr;

    // Has this profile been loaded?
    bool                loaded      = false;

    // Configuration data, loaded from a file, or set from the defaults.
    DynamicPrintConfig  config;

    // Alias of the preset
    std::string         alias;
    // List of profile names, from which this profile was renamed at some point of time.
    // This list is then used to match profiles by their names when loaded from .gcode, .3mf, .amf,
    // and to match the "inherits" field of user profiles with updated system profiles.
    std::vector<std::string> renamed_from;

    void                save();

    // Return a label of this preset, consisting of a name and a "(modified)" suffix, if this preset is dirty.
    std::string         label() const;

    // Set the is_dirty flag if the provided config is different from the active one.
    void                set_dirty(const DynamicPrintConfig &config) { this->is_dirty = ! this->config.diff(config).empty(); }
    void                set_dirty(bool dirty = true) { this->is_dirty = dirty; }
    void                reset_dirty() { this->is_dirty = false; }

    // Returns the name of the preset, from which this preset inherits.
    static std::string& inherits(DynamicPrintConfig &cfg) { return cfg.option<ConfigOptionString>("inherits", true)->value; }
    std::string&        inherits() { return Preset::inherits(this->config); }
    const std::string&  inherits() const { return Preset::inherits(const_cast<Preset*>(this)->config); }

    // Returns the "compatible_prints_condition".
    static std::string& compatible_prints_condition(DynamicPrintConfig &cfg) { return cfg.option<ConfigOptionString>("compatible_prints_condition", true)->value; }
    std::string&        compatible_prints_condition() { 
		assert(this->type == TYPE_FILAMENT || this->type == TYPE_SLA_MATERIAL);
        return Preset::compatible_prints_condition(this->config);
    }
    const std::string&  compatible_prints_condition() const { return const_cast<Preset*>(this)->compatible_prints_condition(); }

    // Returns the "compatible_printers_condition".
    static std::string& compatible_printers_condition(DynamicPrintConfig &cfg) { return cfg.option<ConfigOptionString>("compatible_printers_condition", true)->value; }
    std::string&        compatible_printers_condition() {
		assert(this->type == TYPE_PRINT || this->type == TYPE_SLA_PRINT || this->type == TYPE_FILAMENT || this->type == TYPE_SLA_MATERIAL);
        return Preset::compatible_printers_condition(this->config);
    }
    const std::string&  compatible_printers_condition() const { return const_cast<Preset*>(this)->compatible_printers_condition(); }

    // Return a printer technology, return ptFFF if the printer technology is not set.
    static PrinterTechnology printer_technology(const DynamicPrintConfig &cfg) {
        auto *opt = cfg.option<ConfigOptionEnum<PrinterTechnology>>("printer_technology");
        // The following assert may trigger when importing some legacy profile, 
        // but it is safer to keep it here to capture the cases where the "printer_technology" key is queried, where it should not.
//        assert(opt != nullptr);
        return (opt == nullptr) ? ptFFF : opt->value;
    }
    PrinterTechnology   printer_technology() const { return Preset::printer_technology(this->config); }
    // This call returns a reference, it may add a new entry into the DynamicPrintConfig.
    PrinterTechnology&  printer_technology_ref() { return this->config.option<ConfigOptionEnum<PrinterTechnology>>("printer_technology", true)->value; }

    // Set is_visible according to application config
    void                set_visible_from_appconfig(const AppConfig &app_config);

    // Resize the extruder specific fields, initialize them with the content of the 1st extruder.
    void                set_num_extruders(unsigned int n) { this->config.set_num_extruders(n); }

    // Sort lexicographically by a preset name. The preset name shall be unique across a single PresetCollection.
    bool                operator<(const Preset &other) const { return this->name < other.name; }

    static const std::vector<std::string>&  print_options();
    static const std::vector<std::string>&  filament_options();
    // Printer options contain the nozzle options.
    static const std::vector<std::string>&  printer_options();
    // Nozzle options of the printer options.
    static const std::vector<std::string>&  nozzle_options();
    // Printer machine limits, those are contained in printer_options().
    static const std::vector<std::string>&  machine_limits_options();

    static const std::vector<std::string>&  sla_printer_options();
    static const std::vector<std::string>&  sla_material_options();
    static const std::vector<std::string>&  sla_print_options();

	static void                             update_suffix_modified(const std::string& new_suffix_modified);
    static const std::string&               suffix_modified();
    static std::string                      remove_suffix_modified(const std::string& name);
    static void                             normalize(DynamicPrintConfig &config);
    // Report configuration fields, which are misplaced into a wrong group, remove them from the config.
    static std::string                      remove_invalid_keys(DynamicPrintConfig &config, const DynamicPrintConfig &default_config);

protected:
    friend class        PresetCollection;
    friend class        PresetBundle;
};

bool is_compatible_with_print  (const PresetWithVendorProfile &preset, const PresetWithVendorProfile &active_print, const PresetWithVendorProfile &active_printer);
bool is_compatible_with_printer(const PresetWithVendorProfile &preset, const PresetWithVendorProfile &active_printer, const DynamicPrintConfig *extra_config);
bool is_compatible_with_printer(const PresetWithVendorProfile &preset, const PresetWithVendorProfile &active_printer);

enum class PresetSelectCompatibleType {
	// Never select a compatible preset if the newly selected profile is not compatible.
	Never,
	// Only select a compatible preset if the active profile used to be compatible, but it is no more.
	OnlyIfWasCompatible,
	// Always select a compatible preset if the active profile is no more compatible.
	Always
};

// Substitutions having been performed during parsing a single configuration file.
struct PresetConfigSubstitutions {
    // User readable preset name.
    std::string                             preset_name;
    // Type of the preset (Print / Filament / Printer ...)
    Preset::Type                            preset_type;
    enum class Source {
        UserFile,
        ConfigBundle,
    };
    Source                                  preset_source;
    // Source of the preset. It may be empty in case of a ConfigBundle being loaded.
    std::string                             preset_file;
    // What config value has been substituted with what.
    ConfigSubstitutions                     substitutions;
};

// Substitutions having been performed during parsing a set of configuration files, for example when starting up
// PrusaSlicer and reading the user Print / Filament / Printer profiles.
using PresetsConfigSubstitutions = std::vector<PresetConfigSubstitutions>;

// Collections of presets of the same type (one of the Print, Filament or Printer type).
class PresetCollection
{
public:
    // Initialize the PresetCollection with the "- default -" preset.
    PresetCollection(Preset::Type type, const std::vector<std::string> &keys, const Slic3r::StaticPrintConfig &defaults, const std::string &default_name = "- default -");
    ~PresetCollection();

    typedef std::deque<Preset>::iterator Iterator;
    typedef std::deque<Preset>::const_iterator ConstIterator;
    Iterator        begin() { return m_presets.begin() + m_num_default_presets; }
    ConstIterator   begin() const { return m_presets.cbegin() + m_num_default_presets; }
    ConstIterator   cbegin() const { return m_presets.cbegin() + m_num_default_presets; }
    Iterator        end() { return m_presets.end(); }
    ConstIterator   end() const { return m_presets.cend(); }
    ConstIterator   cend() const { return m_presets.cend(); }

    void            reset(bool delete_files);

    Preset::Type    type() const { return m_type; }
    // Name, to be used on the screen and in error messages. Not localized.
    std::string     name() const;
    // Name, to be used as a section name in config bundle, and as a folder name for presets.
    std::string     section_name() const;
    const std::deque<Preset>& operator()() const { return m_presets; }

    // Add default preset at the start of the collection, increment the m_default_preset counter.
    void            add_default_preset(const std::vector<std::string> &keys, const Slic3r::StaticPrintConfig &defaults, const std::string &preset_name);

    // Load ini files of the particular type from the provided directory path.
    void            load_presets(const std::string &dir_path, const std::string &subdir, PresetsConfigSubstitutions& substitutions, ForwardCompatibilitySubstitutionRule rule);

    // Load a preset from an already parsed config file, insert it into the sorted sequence of presets
    // and select it, losing previous modifications.
    Preset&         load_preset(const std::string &path, const std::string &name, const DynamicPrintConfig &config, bool select = true);
    Preset&         load_preset(const std::string &path, const std::string &name, DynamicPrintConfig &&config, bool select = true);

    // Returns a loaded preset, returns true if an existing preset was selected AND modified from config.
    // In that case the successive filament loaded for a multi material printer should not be modified, but
    // an external preset should be created instead.
    enum class LoadAndSelect {
        // Never select
        Never,
        // Always select
        Always,
        // Select a profile only if it was modified.
        OnlyIfModified,
    };
    std::pair<Preset*, bool> load_external_preset(
        // Path to the profile source file (a G-code, an AMF or 3MF file, a config file)
        const std::string           &path,
        // Name of the profile, derived from the source file name.
        const std::string           &name,
        // Original name of the profile, extracted from the loaded config. Empty, if the name has not been stored.
        const std::string           &original_name,
        // Config to initialize the preset from.
        const DynamicPrintConfig    &config,
        // Select the preset after loading?
        LoadAndSelect                select = LoadAndSelect::Always);

    // Save the preset under a new name. If the name is different from the old one,
    // a new preset is stored into the list of presets.
    // All presets are marked as not modified and the new preset is activated.
    void            save_current_preset(const std::string &new_name, bool detach = false);

    // Delete the current preset, activate the first visible preset.
    // returns true if the preset was deleted successfully.
    bool            delete_current_preset();
    // Delete the current preset, activate the first visible preset.
    // returns true if the preset was deleted successfully.
    bool            delete_preset(const std::string& name);

    // Enable / disable the "- default -" preset.
    void            set_default_suppressed(bool default_suppressed);
    bool            is_default_suppressed() const { return m_default_suppressed; }

    // Select a preset. If an invalid index is provided, the first visible preset is selected.
    Preset&         select_preset(size_t idx);
    // Return the selected preset, without the user modifications applied.
    Preset&         get_selected_preset()       { return m_presets[m_idx_selected]; }
    const Preset&   get_selected_preset() const { return m_presets[m_idx_selected]; }
    size_t          get_selected_idx()    const { return m_idx_selected; }
    // Returns the name of the selected preset, or an empty string if no preset is selected.
    std::string     get_selected_preset_name() const { return (m_idx_selected == size_t(-1)) ? std::string() : this->get_selected_preset().name; }
    // For the current edited preset, return the parent preset if there is one.
    // If there is no parent preset, nullptr is returned.
    // The parent preset may be a system preset or a user preset, which will be
    // reflected by the UI.
    const Preset*   get_selected_preset_parent() const;
	// Get parent preset for a child preset, based on the "inherits" field of a child,
	// where the "inherits" profile name is searched for in both m_presets and m_map_system_profile_renamed.
	const Preset*	get_preset_parent(const Preset& child) const;
	// Return the selected preset including the user modifications.
    Preset&         get_edited_preset()         { return m_edited_preset; }
    const Preset&   get_edited_preset() const   { return m_edited_preset; }

    // Return vendor of the first parent profile, for which the vendor is defined, or null if such profile does not exist.
    PresetWithVendorProfile get_preset_with_vendor_profile(const Preset &preset) const;
    PresetWithVendorProfile get_edited_preset_with_vendor_profile() const { return this->get_preset_with_vendor_profile(this->get_edited_preset()); }

    const std::string& 		get_preset_name_by_alias(const std::string& alias) const;
	const std::string*		get_preset_name_renamed(const std::string &old_name) const;

	// used to update preset_choice from Tab
	const std::deque<Preset>&	get_presets() const	{ return m_presets; }
    size_t                      get_idx_selected()	{ return m_idx_selected; }
	static const std::string&	get_suffix_modified();

    // Return a preset possibly with modifications.
	Preset&			default_preset(size_t idx = 0)		 { assert(idx < m_num_default_presets); return m_presets[idx]; }
	const Preset&   default_preset(size_t idx = 0) const { assert(idx < m_num_default_presets); return m_presets[idx]; }
	virtual const Preset& default_preset_for(const DynamicPrintConfig & /* config */) const { return this->default_preset(); }
    // Return a preset by an index. If the preset is active, a temporary copy is returned.
    Preset&         preset(size_t idx)          { return (idx == m_idx_selected) ? m_edited_preset : m_presets[idx]; }
    const Preset&   preset(size_t idx) const    { return const_cast<PresetCollection*>(this)->preset(idx); }
    void            discard_current_changes()   { m_presets[m_idx_selected].reset_dirty(); m_edited_preset = m_presets[m_idx_selected]; }
    
    // Return a preset by its name. If the preset is active, a temporary copy is returned.
    // If a preset is not found by its name, null is returned.
    Preset*         find_preset(const std::string &name, bool first_visible_if_not_found = false);
    const Preset*   find_preset(const std::string &name, bool first_visible_if_not_found = false) const 
        { return const_cast<PresetCollection*>(this)->find_preset(name, first_visible_if_not_found); }

    size_t          first_visible_idx() const;
    // Return index of the first compatible preset. Certainly at least the '- default -' preset shall be compatible.
    // If one of the prefered_alternates is compatible, select it.
    template<typename PreferedCondition>
    size_t          first_compatible_idx(PreferedCondition prefered_condition) const
    {
        size_t i = m_default_suppressed ? m_num_default_presets : 0;
        size_t n = this->m_presets.size();
        size_t i_compatible = n;
        int    match_quality = -1;
        for (; i < n; ++ i)
            // Since we use the filament selection from Wizard, it's needed to control the preset visibility too 
            if (m_presets[i].is_compatible && m_presets[i].is_visible) {
                int this_match_quality = prefered_condition(m_presets[i]);
                if (this_match_quality > match_quality) {
                    if (match_quality == std::numeric_limits<int>::max())
                        // Better match will not be found.
                        return i;
                    // Store the first compatible profile with highest match quality into i_compatible.
                    i_compatible = i;
                    match_quality = this_match_quality;
                } 
            }
        return (i_compatible == n) ? 
            // No compatible preset found, return the default preset.
            0 :
            // Compatible preset found.
            i_compatible;
    }
    // Return index of the first compatible preset. Certainly at least the '- default -' preset shall be compatible.
    size_t          first_compatible_idx() const { return this->first_compatible_idx([](const Preset&) -> int { return 0; }); }

    // Return index of the first visible preset. Certainly at least the '- default -' preset shall be visible.
    // Return the first visible preset. Certainly at least the '- default -' preset shall be visible.
    Preset&         first_visible()             { return this->preset(this->first_visible_idx()); }
    const Preset&   first_visible() const       { return this->preset(this->first_visible_idx()); }
    Preset&         first_compatible()          { return this->preset(this->first_compatible_idx()); }
    template<typename PreferedCondition>
    Preset&         first_compatible(PreferedCondition prefered_condition) { return this->preset(this->first_compatible_idx(prefered_condition)); }
    const Preset&   first_compatible() const    { return this->preset(this->first_compatible_idx()); }

    // Return number of presets including the "- default -" preset.
    size_t          size() const                { return m_presets.size(); }
    bool            has_defaults_only() const   { return m_presets.size() <= m_num_default_presets; }

    // For Print / Filament presets, disable those, which are not compatible with the printer.
    template<typename PreferedCondition>
    void            update_compatible(const PresetWithVendorProfile &active_printer, const PresetWithVendorProfile *active_print, PresetSelectCompatibleType select_other_if_incompatible, PreferedCondition prefered_condition)
    {
        if (this->update_compatible_internal(active_printer, active_print, select_other_if_incompatible) == (size_t)-1)
            // Find some other compatible preset, or the "-- default --" preset.
            this->select_preset(this->first_compatible_idx(prefered_condition));        
    }
    void            update_compatible(const PresetWithVendorProfile &active_printer, const PresetWithVendorProfile *active_print, PresetSelectCompatibleType select_other_if_incompatible)
        { this->update_compatible(active_printer, active_print, select_other_if_incompatible, [](const Preset&) -> int { return 0; }); }

    size_t          num_visible() const { return std::count_if(m_presets.begin(), m_presets.end(), [](const Preset &preset){return preset.is_visible;}); }

    // Compare the content of get_selected_preset() with get_edited_preset() configs, return true if they differ.
    bool                        current_is_dirty() const { return ! this->current_dirty_options().empty(); }
    // Compare the content of get_selected_preset() with get_edited_preset() configs, return the list of keys where they differ.
    std::vector<std::string>    current_dirty_options(const bool deep_compare = false) const
        { return dirty_options(&this->get_edited_preset(), &this->get_selected_preset(), deep_compare); }
    // Compare the content of get_selected_preset() with get_edited_preset() configs, return the list of keys where they differ.
    std::vector<std::string>    current_different_from_parent_options(const bool deep_compare = false) const
        { return dirty_options(&this->get_edited_preset(), this->get_selected_preset_parent(), deep_compare); }

    // Return a sorted list of system preset names.
    // Used for validating the "inherits" flag when importing user's config bundles.
    // Returns names of all system presets including the former names of these presets.
    std::vector<std::string>    system_preset_names() const;

    // Update a dirty flag of the current preset
    // Return true if the dirty flag changed.
    bool            update_dirty();
    
    // Select a profile by its name. Return true if the selection changed.
    // Without force, the selection is only updated if the index changes.
    // With force, the changes are reverted if the new index is the same as the old index.
    bool            select_preset_by_name(const std::string &name, bool force);

    // Generate a file path from a profile name. Add the ".ini" suffix if it is missing.
    std::string     path_from_name(const std::string &new_name) const;

    size_t num_default_presets() { return m_num_default_presets; }

protected:
    // Select a preset, if it exists. If it does not exist, select an invalid (-1) index.
    // This is a temporary state, which shall be fixed immediately by the following step.
    bool            select_preset_by_name_strict(const std::string &name);

    // Merge one vendor's presets with the other vendor's presets, report duplicates.
    std::vector<std::string> merge_presets(PresetCollection &&other, const VendorMap &new_vendors);

    // Update m_map_alias_to_profile_name from loaded system profiles.
	void 			update_map_alias_to_profile_name();

    // Update m_map_system_profile_renamed from loaded system profiles.
    void 			update_map_system_profile_renamed();

private:
    PresetCollection();
    PresetCollection(const PresetCollection &other);
    PresetCollection& operator=(const PresetCollection &other);

    // Find a preset position in the sorted list of presets.
    // The "-- default -- " preset is always the first, so it needs
    // to be handled differently.
    // If a preset does not exist, an iterator is returned indicating where to insert a preset with the same name.
    std::deque<Preset>::iterator find_preset_internal(const std::string &name)
    {
        auto it = Slic3r::lower_bound_by_predicate(m_presets.begin() + m_num_default_presets, m_presets.end(), [&name](const auto& l) { return l.name < name;  });
        if (it == m_presets.end() || it->name != name) {
            // Preset has not been not found in the sorted list of non-default presets. Try the defaults.
            for (size_t i = 0; i < m_num_default_presets; ++ i)
                if (m_presets[i].name == name) {
                    it = m_presets.begin() + i;
                    break;
                }
        }
        return it;
    }
    std::deque<Preset>::const_iterator find_preset_internal(const std::string &name) const
        { return const_cast<PresetCollection*>(this)->find_preset_internal(name); }
    std::deque<Preset>::iterator 	   find_preset_renamed(const std::string &name) {
    	auto it_renamed = m_map_system_profile_renamed.find(name);
    	auto it = (it_renamed == m_map_system_profile_renamed.end()) ? m_presets.end() : this->find_preset_internal(it_renamed->second);
    	assert((it_renamed == m_map_system_profile_renamed.end()) || (it != m_presets.end() && it->name == it_renamed->second));
    	return it;
    }
    std::deque<Preset>::const_iterator find_preset_renamed(const std::string &name) const
        { return const_cast<PresetCollection*>(this)->find_preset_renamed(name); }

    size_t update_compatible_internal(const PresetWithVendorProfile &active_printer, const PresetWithVendorProfile *active_print, PresetSelectCompatibleType unselect_if_incompatible);

    static std::vector<std::string> dirty_options(const Preset *edited, const Preset *reference, const bool is_printer_type = false);

    // Type of this PresetCollection: TYPE_PRINT, TYPE_FILAMENT or TYPE_PRINTER.
    Preset::Type            m_type;
    // List of presets, starting with the "- default -" preset.
    // Use deque to force the container to allocate an object per each entry, 
    // so that the addresses of the presets don't change during resizing of the container.
    std::deque<Preset>      m_presets;
    // System profiles may have aliases. Map to the full profile name.
    std::vector<std::pair<std::string, std::string>> m_map_alias_to_profile_name;
    // Map from old system profile name to a current system profile name.
    std::map<std::string, std::string> m_map_system_profile_renamed;
    // Initially this preset contains a copy of the selected preset. Later on, this copy may be modified by the user.
    Preset                  m_edited_preset;
    // Selected preset.
    size_t                  m_idx_selected;
    // Is the "- default -" preset suppressed?
    bool                    m_default_suppressed  = true;
    size_t                  m_num_default_presets = 0;

    // Path to the directory to store the config files into.
    std::string             m_dir_path;

    // to access select_preset_by_name_strict()
    friend class PresetBundle;
};

// Printer supports the FFF and SLA technologies, with different set of configuration values,
// therefore this PresetCollection needs to handle two defaults.
class PrinterPresetCollection : public PresetCollection
{
public:
    PrinterPresetCollection(Preset::Type type, const std::vector<std::string> &keys, const Slic3r::StaticPrintConfig &defaults, const std::string &default_name = "- default -") :
		PresetCollection(type, keys, defaults, default_name) {}
    const Preset&   default_preset_for(const DynamicPrintConfig &config) const override;

    const Preset*   find_by_model_id(const std::string &model_id) const;
};

namespace PresetUtils {
	// PrinterModel of a system profile, from which this preset is derived, or null if it is not derived from a system profile.
	const VendorProfile::PrinterModel* system_printer_model(const Preset &preset);
    std::string system_printer_bed_model(const Preset& preset);
    std::string system_printer_bed_texture(const Preset& preset);
} // namespace PresetUtils


//////////////////////////////////////////////////////////////////////

class PhysicalPrinter
{
public:
    PhysicalPrinter(const std::string& name, const DynamicPrintConfig &default_config);
    PhysicalPrinter(const std::string& name, const DynamicPrintConfig &default_config, const Preset& preset);
    void set_name(const std::string &name);

    // Name of the Physical Printer, usually derived form the file name.
    std::string         name;
    // File name of the Physical Printer.
    std::string         file;
    // Configuration data, loaded from a file, or set from the defaults.
    DynamicPrintConfig  config;
    // set of presets used with this physical printer
    std::set<std::string> preset_names;

    // Has this profile been loaded?
    bool                loaded = false;

    static std::string  separator();
    static const std::vector<std::string>&  printer_options();
    static const std::vector<std::string>&  print_host_options();
    static std::vector<std::string>         presets_with_print_host_information(const PrinterPresetCollection& printer_presets);
    static bool has_print_host_information(const DynamicPrintConfig& config);

    const std::set<std::string>&            get_preset_names() const;

    bool                has_empty_config() const;
    void                update_preset_names_in_config();

    void                save() { this->config.save(this->file); }
    void                save(const std::string& file_name_from, const std::string& file_name_to);

    void                update_from_preset(const Preset& preset);
    void                update_from_config(const DynamicPrintConfig &new_config);

    // add preset to the preset_names
    // return false, if preset with this name is already exist in the set
    bool                add_preset(const std::string& preset_name);
    bool                delete_preset(const std::string& preset_name);
    void                reset_presets();

    // Return a printer technology, return ptFFF if the printer technology is not set.
    static PrinterTechnology printer_technology(const DynamicPrintConfig& cfg) {
        auto* opt = cfg.option<ConfigOptionEnum<PrinterTechnology>>("printer_technology");
        // The following assert may trigger when importing some legacy profile, 
        // but it is safer to keep it here to capture the cases where the "printer_technology" key is queried, where it should not.
        return (opt == nullptr) ? ptFFF : opt->value;
    }
    PrinterTechnology   printer_technology() const { return printer_technology(this->config); }

    // Sort lexicographically by a preset name. The preset name shall be unique across a single PresetCollection.
    bool                operator<(const PhysicalPrinter& other) const { return this->name < other.name; }

    // get full printer name included a name of the preset
    std::string         get_full_name(std::string preset_name) const;

    // get printer name from the full name uncluded preset name
    static std::string  get_short_name(std::string full_name);

    // get preset name from the full name uncluded printer name
    static std::string  get_preset_name(std::string full_name);

protected:
    friend class        PhysicalPrinterCollection;
};


// ---------------------------------
// ***  PhysicalPrinterCollection  ***
// ---------------------------------

// Collections of physical printers
class PhysicalPrinterCollection
{
public:
    PhysicalPrinterCollection(const std::vector<std::string>& keys);
    ~PhysicalPrinterCollection() {}

    typedef std::deque<PhysicalPrinter>::iterator Iterator;
    typedef std::deque<PhysicalPrinter>::const_iterator ConstIterator;
    Iterator        begin() { return m_printers.begin(); }
    ConstIterator   begin() const { return m_printers.cbegin(); }
    ConstIterator   cbegin() const { return m_printers.cbegin(); }
    Iterator        end() { return m_printers.end(); }
    ConstIterator   end() const { return m_printers.cend(); }
    ConstIterator   cend() const { return m_printers.cend(); }

    bool            empty() const {return m_printers.empty(); }

    void            reset(bool delete_files) {};

    const std::deque<PhysicalPrinter>& operator()() const { return m_printers; }

    // Load ini files of the particular type from the provided directory path.
    void            load_printers(const std::string& dir_path, const std::string& subdir, PresetsConfigSubstitutions& substitutions, ForwardCompatibilitySubstitutionRule rule);
    void            load_printers_from_presets(PrinterPresetCollection &printer_presets);
    // Load printer from the loaded configuration
    void            load_printer(const std::string& path, const std::string& name, DynamicPrintConfig&& config, bool select, bool save=false);

    // Save the printer under a new name. If the name is different from the old one,
    // a new printer is stored into the list of printers.
    // New printer is activated.
    void            save_printer(PhysicalPrinter& printer, const std::string& renamed_from = "");

    // Delete the current preset, activate the first visible preset.
    // returns true if the preset was deleted successfully.
    bool            delete_printer(const std::string& name);
    // Delete the selected preset
    // returns true if the preset was deleted successfully.
    bool            delete_selected_printer();
    // Delete preset_name preset from all printers:
    // If there is last preset for the printer and first_check== false, then delete this printer
    // returns true if all presets were deleted successfully.
    bool            delete_preset_from_printers(const std::string& preset_name);

    // Get list of printers which have more than one preset and "preset_name" preset is one of them
    std::vector<std::string> get_printers_with_preset( const std::string &preset_name);
    // Get list of printers which has only "preset_name" preset
    std::vector<std::string> get_printers_with_only_preset( const std::string &preset_name);

    // Return the selected preset, without the user modifications applied.
    PhysicalPrinter&        get_selected_printer() { return m_printers[m_idx_selected]; }
    const PhysicalPrinter&  get_selected_printer() const { return m_printers[m_idx_selected]; }

    size_t                  get_selected_idx()    const { return m_idx_selected; }
    // Returns the name of the selected preset, or an empty string if no preset is selected.
    std::string             get_selected_printer_name() const { return (m_idx_selected == size_t(-1)) ? std::string() : this->get_selected_printer().name; }
    // Returns the config of the selected printer, or nullptr if no printer is selected.
    DynamicPrintConfig*     get_selected_printer_config() { return (m_idx_selected == size_t(-1)) ? nullptr : &(this->get_selected_printer().config); }
    // Returns the config of the selected printer, or nullptr if no printer is selected.
    PrinterTechnology       get_selected_printer_technology() { return (m_idx_selected == size_t(-1)) ? PrinterTechnology::ptAny : this->get_selected_printer().printer_technology(); }

    // Each physical printer can have a several related preset,
    // so, use the next functions to get an exact names of selections in the list:
    // Returns the full name of the selected printer, or an empty string if no preset is selected.
    std::string     get_selected_full_printer_name() const;
    // Returns the printer model of the selected preset, or an empty string if no preset is selected.
    std::string     get_selected_printer_preset_name() const { return (m_idx_selected == size_t(-1)) ? std::string() : m_selected_preset; }

    // Select printer by the full printer name, which contains name of printer, separator and name of selected preset
    // If full_name doesn't contain name of selected preset, then select first preset in the list for this printer
    void select_printer(const std::string& full_name);
    void select_printer(const PhysicalPrinter& printer);
    void select_printer(const std::string& printer_name, const std::string& preset_name);
    bool has_selection() const;
    void unselect_printer() ;
    bool is_selected(ConstIterator it, const std::string &preset_name) const;

    // Return a printer by an index. If the printer is active, a temporary copy is returned.
    PhysicalPrinter& printer(size_t idx) { return m_printers[idx]; }
    const PhysicalPrinter& printer(size_t idx) const { return const_cast<PhysicalPrinterCollection*>(this)->printer(idx); }

    // Return a preset by its name. If the preset is active, a temporary copy is returned.
    // If a preset is not found by its name, null is returned.
    // It is possible case (in)sensitive search
    PhysicalPrinter* find_printer(const std::string& name, bool case_sensitive_search = true);
    const PhysicalPrinter* find_printer(const std::string& name, bool case_sensitive_search = true) const
    {
        return const_cast<PhysicalPrinterCollection*>(this)->find_printer(name, case_sensitive_search);
    }

    // Generate a file path from a profile name. Add the ".ini" suffix if it is missing.
    std::string     path_from_name(const std::string& new_name) const;

    const DynamicPrintConfig& default_config() const { return m_default_config; }

private:
    PhysicalPrinterCollection& operator=(const PhysicalPrinterCollection& other);

    // Find a physical printer position in the sorted list of printers.
    // The name of a printer should be unique and case insensitive
    // Use this functions with case_sensitive_search = false, when you need case insensitive search
    std::deque<PhysicalPrinter>::iterator find_printer_internal(const std::string& name, bool case_sensitive_search = true);
    std::deque<PhysicalPrinter>::const_iterator find_printer_internal(const std::string& name, bool case_sensitive_search = true) const
    {
        return const_cast<PhysicalPrinterCollection*>(this)->find_printer_internal(name);
    }

    PhysicalPrinter* find_printer_with_same_config( const DynamicPrintConfig &config);

    // List of printers
    // Use deque to force the container to allocate an object per each entry, 
    // so that the addresses of the presets don't change during resizing of the container.
    std::deque<PhysicalPrinter> m_printers;

    // Default config for a physical printer containing all key/value pairs of PhysicalPrinter::printer_options().
    DynamicPrintConfig          m_default_config;

    // Selected printer.
    size_t                      m_idx_selected = size_t(-1);
    // The name of the preset which is currently select for this printer
    std::string                 m_selected_preset;

    // Path to the directory to store the config files into.
    std::string                 m_dir_path;
};


} // namespace Slic3r

#endif /* slic3r_Preset_hpp_ */
